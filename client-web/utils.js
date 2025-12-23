function setCookie(key, value) {
      document.cookie = `${key}=${value}; path=/`; // might have alot of problems here with vulnerabilities
    }
    function getCookie(key) {
      let cookies = document.cookie;
      return cookies
        .substring(cookies.findIndex(key) + key.length)
        .split(";")[0];
    }
    function deleteCookie(key) {
      document.cookie = `${key}=; expires=Thu, 01 Jan 1970 00:00:00 UTC;`;
    }