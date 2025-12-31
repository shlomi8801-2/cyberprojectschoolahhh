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
      const container = document.createElement("table")
      var row = document.createElement("tr")
      document.getElementById("container").appendChild(container)
      container.appendChild(row)
      for (var x=0;x<res.columns.length;x++){
                var elem = document.createElement("th");
          elem.innerText=res.columns[x]
          row.appendChild(elem)
      }
      for (var x=0;x<res.users.length;++x){
        row = document.createElement("tr")
        for (var i=0;i<res.users[i].length;i++){
          var elem = document.createElement("td");
          elem.innerText=res.users[x][i];
          row.appendChild(elem)
        }
        container.appendChild(row)
      }
      break;
    default:
      alert("error getting users list: unexpected code");
  }
}
getUsers()