async function getUsers() {
    const maxrows=100;
    const offset = 0;
  var res = await fetch(API_URL + `/list/users/${maxrows}/${offset}`, {
    method: "get",
    headers: {"Token":getCookie("token")},
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
          if (column.includes("username") && 0){ // currently "&& 0" because for now its skipping it
            //translate from base64
          }else if (column.includes("date")){
            const tmpElem = document.createElement("input")
            tmpElem.type = "date";
            tmpElem.disabled = true
            tmpElem.value =epocToYYYYMMDD(currentDataValue);
            console.log(tmpElem.value)
            valueElement = tmpElem;
          }else{
            valueElement = document.createElement("a")
            valueElement.innerText = currentDataValue;
          }
          
          elem.appendChild(valueElement);
          row.appendChild(elem)
        }
        container.appendChild(row)
      }
      break;
    default:
      alert("error getting users list: unexpected code");
  }
}

function epocToYYYYMMDD(epocTimeInSeconds){
  const epocTime = epocTimeInSeconds*1000
  return (new Date(Number(epocTime))).toISOString().substring(0,10); // it returns something like '1970-01-01T00:00:00.001Z' so we cut it to 10 first digits(thats what date input takes)
}











getUsers()