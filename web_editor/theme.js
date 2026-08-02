(function () {
  var THEME_KEY = 'sf-theme';

  function getTheme() {
    try { return localStorage.getItem(THEME_KEY) || 'dark'; } catch (e) { return 'dark'; }
  }

  function apply(theme) {
    document.documentElement.setAttribute('data-theme', theme);
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

  window.AppTheme = {
    get: getTheme,
    set: setTheme,
    toggle: function () { setTheme(getTheme() === 'light' ? 'dark' : 'light'); }
  };
})();
