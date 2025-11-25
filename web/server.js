// ...existing code...
const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const { spawn } = require('child_process');
const path = require('path');
const fs = require('fs');

const app = express();
app.use(cors());
app.use(bodyParser.json());

const DUANYAO_EXEC = path.resolve(__dirname, '..', 'duanyao');
const MAX_FIELD_LEN = 2000;
const MAX_OUTPUT_BYTES = 1024 * 1024;
const PROCESS_TIMEOUT_MS = 20000;

function clamp(s){ s = (s===undefined||s===null)?'':String(s); return s.length>MAX_FIELD_LEN? s.slice(0,MAX_FIELD_LEN) : s; }
function ensureExecutableExists(res){
  if (!fs.existsSync(DUANYAO_EXEC)){
    const msg = `duanyao executable not found: ${DUANYAO_EXEC}\n请在 /home/oldfather/duanyao 运行 make 生成可执行文件后重试。`;
    if (res) res.status(500).json({ success:false, error: msg });
    return false;
  }
  return true;
}

app.post('/api/run', (req, res) => {
  if (!ensureExecutableExists(res)) return;

  const safe = {};
  ['hand','hupai','dora','ldora','changfeng','zifeng','flags'].forEach(k => safe[k] = clamp(req.body[k]));

  // try { fs.writeFileSync(path.join(__dirname, `payload.${Date.now()}.json`), JSON.stringify(safe, null, 2)); } catch(e){}

  const inputLines = [
    safe.hand, safe.hupai, safe.changfeng, safe.zifeng, safe.dora, safe.flags || '', safe.ldora
  ];
  inputLines.push('1');

  let proc;
  try {
    proc = spawn(DUANYAO_EXEC, [], { cwd: path.dirname(DUANYAO_EXEC) });
  } catch (err) {
    return res.status(500).json({ success:false, error: 'spawn failed: ' + String(err) });
  }

  let stdout = '', stderr = '';
  let killedByServer = false;

  proc.on('error', err => {
    if (!res.headersSent) res.status(500).json({ success:false, error: String(err) });
  });

  proc.stdout.on('data', chunk => {
    stdout += chunk.toString();
    if (stdout.length > MAX_OUTPUT_BYTES) {
      killedByServer = true;
      proc.kill('SIGKILL');
      stderr += '\n[server] stdout exceeded max size and was killed';
    }
  });

  proc.stderr.on('data', chunk => {
    stderr += chunk.toString();
    if (stderr.length > MAX_OUTPUT_BYTES) {
      killedByServer = true;
      proc.kill('SIGKILL');
      stderr += '\n[server] stderr exceeded max size and was killed';
    }
  });

  const to = setTimeout(() => {
    if (!proc.killed) { killedByServer = true; proc.kill('SIGKILL'); stderr += '\n[server] process timeout and was killed'; }
  }, PROCESS_TIMEOUT_MS);

  proc.on('close', (code, signal) => {
    clearTimeout(to);
    if (killedByServer) {
      if (!res.headersSent) res.status(500).json({ success:false, error: stderr, stdout });
      return;
    }

    // 尝试从 stdout 中提取第一个 JSON 对象并解析
    const s = (stdout || '').trim();
    let parsed = null;
    if (s) {
      try {
        // 直接整体解析优先
        parsed = JSON.parse(s);
      } catch (e) {
        const m = s.match(/\{[\s\S]*\}/);
        if (m) {
          try { parsed = JSON.parse(m[0]); } catch (e2) { parsed = null; }
        }
      }
    }

    if (parsed) {
      // 构造期望的中文文本输出格式
     let formatted = '';
      formatted += '役种:\n';
      if (Array.isArray(parsed.yizhong) && parsed.yizhong.length > 0) {
        for (const item of parsed.yizhong) {
          const name = item.name || '';
          const fans = (typeof item.fans === 'number') ? item.fans : item.fans || '';
          formatted += `\t${name} ${fans}番\n`;
        }
      } else {
        formatted += '无\n';
      }

      // fanshu / fushu 行
      if (parsed.fanshu !== undefined && parsed.fushu !== undefined) {
        formatted += `${parsed.fanshu}番 ${parsed.fushu}符\n`;
      }

      // titles 与 score 行（例如 "倍满 16000点"）
      if (Array.isArray(parsed.titles) && parsed.titles.length > 0) {
        formatted += `${parsed.titles.join(' ')} ${parsed.score}点\n`;
      } else if (parsed.score !== undefined) {
        formatted += `${parsed.score}点\n`;
      }

      if (!res.headersSent) res.json({ success:true, data: parsed, formatted, raw_stdout: stdout, stderr });
    } else {
      // 无法解析为 JSON，返回原始输出供前端显示
      if (!res.headersSent) res.status(500).json({ success:false, error: 'failed to parse JSON from duanyao stdout', stdout, stderr, exitCode: code, signal });
    }
  });

  try {
    for (const l of inputLines) proc.stdin.write(Buffer.from((l || '') + '\n', 'utf8'));
    proc.stdin.end();
  } catch (e) {
    if (!res.headersSent) res.status(500).json({ success:false, error: String(e) });
  }
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});
// ...existing code...