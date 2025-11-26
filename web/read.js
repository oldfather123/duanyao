// ...existing code...
const windMap = { "东":"1z", "南":"2z", "西":"3z", "北":"4z" };

function getCheckedRadioValue(name){
  const r = document.querySelector(`input[name="${name}"]:checked`);
  return r ? r.value : "";
}

function buildFlags(){
  const keys = ['wlizhi','lizhi','yifa','zimo','fulu','qianggang','lingshang','hedi','haidi'];
  return keys.map(k => {
    const el = document.querySelector(`input[name="${k}"]`);
    return (el && el.checked) ? '1' : '0';
  }).join('');
}

function clamp(s, max=2000){ s = (s===undefined||s===null)?'':String(s); return s.length>max ? s.slice(0,max) : s; }

async function postOnce(payload, timeout = 20000){
  const controller = new AbortController();
  const id = setTimeout(()=>controller.abort(), timeout);
  try {
    const resp = await fetch('http://127.0.0.1:3000/api/run', {
      method: 'POST',
      headers: {'Content-Type':'application/json'},
      body: JSON.stringify(payload),
      signal: controller.signal
    });
    clearTimeout(id);
    if (!resp.ok) {
      const t = await resp.text();
      throw new Error(`HTTP ${resp.status}: ${t}`);
    }
    return await resp.json();
  } catch (err) {
    clearTimeout(id);
    throw err;
  }
}

document.addEventListener('DOMContentLoaded', () => {
  const btn = document.getElementById('run');
  if (!btn) return;
  btn.addEventListener('click', async () => {
    const payload = {
      hand: clamp(document.getElementById('hand').value.trim()),
      hupai: clamp(document.getElementById('hupai').value.trim()),
      dora: clamp(document.getElementById('dora').value.trim()),
      ldora: clamp(document.getElementById('ldora').value.trim()),
      changfeng: clamp(windMap[getCheckedRadioValue('changfeng')] || ""),
      zifeng: clamp(windMap[getCheckedRadioValue('zifeng')] || ""),
      flags: clamp(buildFlags())
    };

    const outEl = document.getElementById('out');
    if (outEl) outEl.textContent = '请求中...\n' + JSON.stringify(payload, null, 2) + '\n\n';

    try {
      const res = await postOnce(payload, 30000);
      if (res && res.success) {
        // 优先显示后端格式化的中文输出（formatted），没有则显示 data 的 JSON
        if (res.formatted) {
          outEl.textContent = res.formatted;
        } else if (res.data) {
          outEl.textContent = JSON.stringify(res.data, null, 2);
        } else {
          // 若后端仅返回 raw_stdout 或 stderr，显示这些内容
          outEl.textContent = '';
          if (res.raw_stdout) outEl.textContent += 'stdout:\n' + res.raw_stdout + '\n';
          if (res.stderr) outEl.textContent += '\nstderr:\n' + res.stderr + '\n';
          if (!outEl.textContent) outEl.textContent = '后端返回空响应';
        }
      } else {
        if (res) outEl.textContent = '后端返回错误：\n' + JSON.stringify(res, null, 2);
        else outEl.textContent = '后端返回空响应';
      }
    } catch (err) {
      if (outEl) outEl.textContent += '\nread网络或服务器错误: ' + (err.message || String(err));
    }
  });
});
// ...existing code...