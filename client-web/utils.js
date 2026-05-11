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
function createElementFromHTML(htmlString) {
  //credit to Crescent Fresh and Jay Taylor https://stackoverflow.com/questions/494143/creating-a-new-dom-element-from-an-html-string-using-built-in-dom-methods-or-pro
  var div = document.createElement('div');
  div.innerHTML = htmlString.trim();

  // Change this to div.childNodes to support multiple top-level nodes.
  return div.firstChild;
}
async function getUsers() {
  //should return something like this
//   {
//     "code": 0,
//     "columns": [
//         "username",
//         "permissions_level",
//         "token_date",
//         "date_created"
//     ],
//     "users": [
//         [
//             "shlomi",
//             1,
//             1778499547,
//             1769005694
//         ]
//     ]
// }
    const maxrows=100;
    const offset = 0;
    var filters = JSON.stringify({}) // exmaple: {"username":"c2hsb21p"}
  var res = await fetch(API_URL + `/list/users/${maxrows}/${offset}`, {
    method: "get",
    headers: {"Token":getCookie("token"),"Filters":filters},
    credentials: 'include' // not working for some reason may fix later for now using in header
  });
  res = await res.json();
  switch (res.code) {
    case 1: //error
      alert("error gettings users list");
      console.error(res.error)
      break;
    case 0: // success
      //put them in the table
      if (!res.users) {
        alert("users were not found in the server response")
        break;
      }
      if (!res.columns) {
        alert("columns were not found in the server response")
        break;
      }
      return res
      break;
    default:
      alert("error getting users list: unexpected code");
  }
  return null
}
function closeParent(elem){
  //meant for the little x on top of a form(dialog)
  while (elem.tagName!=="DIALOG"){
    elem = elem.parentNode
  }
  elem.open=false
}
function clearChildElements(elem){
   while (elem.firstChild) { // clear childs in btnGrid
        elem.removeChild(elem.firstChild);
    }
}