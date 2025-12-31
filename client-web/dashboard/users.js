async function getUsers() {
    const maxrows=100;
    const offset = 0;
  var res = await fetch(API_URL + `/list/users/${maxrows}`, {
    method: "get",
    headers: {},
    credentials: 'include'
  });
  res = await res.json();
  switch (res.code) {
    case 1: //error
      alert("error gettings users list");
      console.error(res.error)
      break;
    case 0: // success
      //put them in the table
      console.log(res.users)
      const container = document.getElementById("container")
      for (var x=0;x<res.users.length;++x){
        var elem = document.createElement("a");
        elem.innerText=res.users[x];
        container.appendChild(elem)
      }
      break;
    default:
      alert("error getting users list: unexpected code");
  }
}
getUsers()