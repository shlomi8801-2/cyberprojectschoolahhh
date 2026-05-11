async function getUsers() {
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
      displayUsers(res)
      break;
    default:
      alert("error getting users list: unexpected code");
  }
}
function displayUsers(res){
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
        
        for (var i=0;i<res.columns.length;i++){ // columns
          var elem = document.createElement("td");
          //here parse the value acording to the column
          const column = res.columns[i]
          var currentDataValue = res.users[x][i]
          var valueElement;
          if (column.includes("username") && 0){ // currently "&& 0" because for now it doesn't use base64
            //translate from base64
          }else if (column.includes("date")){
            const tmpElem = document.createElement("input")
            tmpElem.type = "date";
            tmpElem.disabled = true
            tmpElem.value =epocToYYYYMMDD(currentDataValue);
            console.log(tmpElem.value)
            valueElement = tmpElem;
          }else{
            // put as is for other
            valueElement = document.createElement("a")
            valueElement.innerText = currentDataValue;
          }
          
          elem.appendChild(valueElement);
          row.appendChild(elem)
        }
        container.appendChild(row)
      }
}

function epocToYYYYMMDD(epocTimeInSeconds){
  const epocTime = epocTimeInSeconds*1000
  return (new Date(Number(epocTime))).toISOString().substring(0,10); // it returns something like '1970-01-01T00:00:00.001Z' so we cut it to 10 first digits(thats what date input takes)
}











getUsers()