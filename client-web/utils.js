function setCookie(key, value) {
  document.cookie = `${key}=${value}; path=/`; // might have alot of problems here with vulnerabilities
}
function getCookie(cname) {
  //from w3schools
  let name = cname + "=";
  let decodedCookie = decodeURIComponent(document.cookie);
  let ca = decodedCookie.split(';');
  for(let i = 0; i <ca.length; i++) {
    let c = ca[i];
    while (c.charAt(0) == ' ') {
      c = c.substring(1);
    }
    if (c.indexOf(name) == 0) {
      return c.substring(name.length, c.length);
    }
  }
  return "";
}
function deleteCookie(key) {
  document.cookie = `${key}=; expires=Thu, 01 Jan 1970 00:00:00 UTC;`;
}
function getCurrentHash() {
  // the fragment of the url for example: "https://example.com/#hello" -> hello
  return new URL(window.location.href).hash;
}
function changeTheme(){
 //change the  data-theme property of the html element of the whole page to "light" or "dark"
}