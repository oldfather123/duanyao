const BACKEND = 'http://127.0.0.1:3000';

async function postAuth(action, username, password) {
  const payload = { action, username, password };
  const resp = await fetch(`${BACKEND}/api/auth`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(payload)
  });

  if (!resp.ok) {
    const text = await resp.text();
    throw new Error(`HTTP ${resp.status}: ${text}`);
  }

  return await resp.json();
}

document.addEventListener('DOMContentLoaded', () => {
  const loginBtn = document.getElementById('loginBtn');
  const regBtn = document.getElementById('regBtn');

  if (loginBtn) {
    loginBtn.addEventListener('click', async () => {
      const username = document.getElementById('loginUser').value.trim();
      const password = document.getElementById('loginPass').value.trim();
      const loginMsg = document.getElementById('loginMsg');

      if (!username || !password) {
        loginMsg.textContent = '用户名和密码不能为空';
        return;
      }

      try {
        const result = await postAuth('login', username, password);
        if (result.success) {
          window.location.href = 'mainpage.html';
        } else {
          loginMsg.textContent = result.error || '登录失败';
        }
      } catch (err) {
        loginMsg.textContent = '网络或服务器错误: ' + err.message;
      }
    });

    const showRegBtn = document.getElementById('showRegBtn');
    if (showRegBtn) {
      showRegBtn.addEventListener('click', () => {
        window.location.href = 'register.html'; // 跳转到注册页面
      });
    }
  }

  if (regBtn) {
    regBtn.addEventListener('click', async () => {
      const username = document.getElementById('regUser').value.trim();
      const password = document.getElementById('regPass').value.trim();
      const regMsg = document.getElementById('regMsg');

      if (!username || !password) {
        regMsg.textContent = '用户名和密码不能为空';
        return;
      }

      try {
        const result = await postAuth('register', username, password);
        if (result.success) {
          window.location.href = 'login.html';
        } else {
          regMsg.textContent = result.error || '注册失败';
        }
      } catch (err) {
        regMsg.textContent = '网络或服务器错误: ' + err.message;
      }
    });

    const cancelRegBtn = document.getElementById('cancelRegBtn');
    if (cancelRegBtn) {
      cancelRegBtn.addEventListener('click', () => {
        window.location.href = 'login.html'; // 取消注册跳转回登录页面
      });
    }
  }
});