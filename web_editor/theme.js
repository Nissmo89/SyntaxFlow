(function () {
  var THEME_KEY = 'sf-theme';
  var ICONS = {
    dark: '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" width="16" height="16"><circle cx="12" cy="12" r="5"/><line x1="12" y1="1" x2="12" y2="3"/><line x1="12" y1="21" x2="12" y2="23"/><line x1="4.22" y1="4.22" x2="5.64" y2="5.64"/><line x1="18.36" y1="18.36" x2="19.78" y2="19.78"/><line x1="1" y1="12" x2="3" y2="12"/><line x1="21" y1="12" x2="23" y2="12"/><line x1="4.22" y1="19.78" x2="5.64" y2="18.36"/><line x1="18.36" y1="5.64" x2="19.78" y2="4.22"/></svg>',
    light: '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" width="16" height="16"><path d="M21 12.79A9 9 0 1 1 11.21 3 7 7 0 0 0 21 12.79z"/></svg>'
  };

  function getTheme() {
    try { return localStorage.getItem(THEME_KEY) || 'dark'; } catch (e) { return 'dark'; }
  }

  function apply(theme) {
    document.documentElement.setAttribute('data-theme', theme);
    var btn = document.getElementById('themeToggle');
    if (btn) {
      var icon = ICONS[theme === 'light' ? 'light' : 'dark'];
      var iconEl = btn.querySelector('.theme-icon') || btn;
      iconEl.innerHTML = icon;
      btn.title = theme === 'light' ? 'Switch to dark theme' : 'Switch to light theme';
    }
    document.dispatchEvent(new CustomEvent('apptheme', { detail: theme }));
  }

  function setTheme(theme) {
    try { localStorage.setItem(THEME_KEY, theme); } catch (e) {}
    apply(theme);
  }

  apply(getTheme());

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', function () { apply(getTheme()); });
  }

  window.addEventListener('storage', function (e) {
    if (e.key === THEME_KEY && e.newValue) apply(e.newValue);
  });

  document.addEventListener('click', function (e) {
    var t = e.target;
    if (t && t.closest && t.closest('#themeToggle')) {
      setTheme(getTheme() === 'light' ? 'dark' : 'light');
    }
  });

  window.AppTheme = {
    get: getTheme,
    set: setTheme,
    toggle: function () { setTheme(getTheme() === 'light' ? 'dark' : 'light'); }
  };
})();
