const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const { spawn } = require('child_process');
const path = require('path');
const fs = require('fs');

const app = express();
app.use(cors());
app.use(bodyParser.json());
app.use(express.static(__dirname));

const DUANYAO_EXEC = path.resolve(__dirname, '..', 'duanyao');
const LOGIN_EXEC = path.resolve(__dirname, '..', 'login');
const MAX_OUTPUT_BYTES = 1024 * 1024;
const PROCESS_TIMEOUT_MS = 20000;

// Helper function to ensure the executable exists
function ensureExecutableExists(res) {
  if (!fs.existsSync(DUANYAO_EXEC)) {
    const msg = `duanyao executable not found: ${DUANYAO_EXEC}\n请在 /home/oldfather/duanyao 运行 make 生成可执行文件后重试。`;
    if (res) res.status(500).json({ success: false, error: msg });
    return false;
  }
  return true;
}

function runExec(execPath, input, res) {
  if (!fs.existsSync(execPath)) {
    return res.status(500).json({ success: false, error: 'executable not found: ' + execPath });
  }
  let proc;
  try {
    proc = spawn(execPath, [], { cwd: path.dirname(execPath) });
  } catch (err) {
    return res.status(500).json({ success: false, error: 'spawn failed: ' + String(err) });
  }

  let stdout = '', stderr = '';
  let killedByServer = false;

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

  const timeout = setTimeout(() => {
    if (!proc.killed) {
      killedByServer = true;
      proc.kill('SIGKILL');
      stderr += '\n[server] process timeout and was killed';
    }
  }, PROCESS_TIMEOUT_MS);

  proc.on('close', (code, signal) => {
    clearTimeout(timeout);
    if (killedByServer) {
      if (!res.headersSent) res.status(500).json({ success: false, error: stderr, stdout });
      return;
    }

    const s = (stdout || '').trim();
    let parsed = null;
    if (s) {
      try {
        parsed = JSON.parse(s);
      } catch (e) {
        const m = s.match(/\{[\s\S]*?\}/);
        if (m) {
          try { parsed = JSON.parse(m[0]); } catch (e2) { parsed = null; }
        }
      }
    }

    if (parsed) {
      res.json(parsed);
    } else {
      res.status(500).json({ success: false, error: 'failed to parse JSON from exec stdout', stdout, stderr, exitCode: code, signal });
    }
  });

  try {
    proc.stdin.write(input + '\n');
    proc.stdin.end();
  } catch (e) {
    if (!res.headersSent) res.status(500).json({ success: false, error: String(e) });
  }
}

function clamp(v) {
  if (v === undefined || v === null) return '';
  if (typeof v === 'string') return v;
  try { return String(v); } catch (e) { return ''; }
}

function savePayload(prefix, payload) {
  try {
    const dir = path.resolve(__dirname, 'payloads');
    if (!fs.existsSync(dir)) fs.mkdirSync(dir, { recursive: true });
    const fname = path.join(dir, `${prefix}_${Date.now()}.json`);
    fs.writeFile(fname, payload, 'utf8', (err) => {
      if (err) console.error('[server] savePayload error:', err);
    });
  } catch (e) {
    console.error('[server] savePayload exception:', e);
  }
}

// API for login and register
app.post('/api/auth', (req, res) => {
  if (!ensureExecutableExists(res)) return;

  const action = req.body.action;
  const username = req.body.username;
  const password = req.body.password;

  if (!action || !username || !password) {
    return res.status(400).json({ success: false, error: 'Missing fields' });
  }

  const payload = JSON.stringify({ action, username, password });
  runExec(LOGIN_EXEC, payload, res);
});

// API for main page (run calculations)
app.post('/api/run', (req, res) => {
  if (!ensureExecutableExists(res)) return;

  const safe = {};
  ['hand','hupai','dora','ldora','changfeng','zifeng','flags'].forEach(k => safe[k] = clamp(req.body[k]));

  // try { fs.writeFileSync(path.join(__dirname, `payload.${Date.now()}.json`), JSON.stringify(safe, null, 2)); } catch(e){}

  const inputLines = [
    safe.hand, safe.hupai, safe.changfeng, safe.zifeng, safe.dora, safe.flags || '', safe.ldora
  ];
  inputLines.push(1);

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
        const m = s.match(/\{[\s\S]*?\}/);
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
          const fans = (typeof item.fans === 'number') ? item.fans : (item.fans || '');
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
app.listen(PORT, '0.0.0.0', () => {
  console.log(`Server is running on http://127.0.0.1:${PORT}`);
});