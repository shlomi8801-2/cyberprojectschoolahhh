async function getClients() { // controllers
  const maxrows = 100;
  const offset = 0;
  var res = await fetch(API_URL + `/list/controllers/${maxrows}/${offset}`, {
    method: "get",
    headers: { Token: getCookie("token") },
    credentials: "include", // not working for some reason may fix later for now using in header
  });
  res = await res.json();
    switch (res.code) {
    case 1: //error
      alert("error gettings users list");
      console.error(res.error)
      break;
    case 0: // success
      //put them in the table
      
      if (!res.controllers) {
        alert("controllers were not found in the server response")
        break;
      }
      if (!res.columns) {
        alert("columns were not found in the server response")
        break;
      }
      displayClients(res)
      break;
    default:
      alert("error getting users list: unexpected code");
  }
  return res;
}

function displayClients(res){
  const container = document.createElement("table")
      var row = document.createElement("tr")
      document.getElementById("container").appendChild(container)
      container.appendChild(row)
      for (var x=0;x<res.columns.length;x++){
                var elem = document.createElement("th");
          elem.innerText=res.columns[x]
          row.appendChild(elem)
      }
      for (var x=0;x<res.controllers.length;++x){
        row = document.createElement("tr")
        const uuidInputElement = createElementFromHTML(`<input type=hidden id="uuid">`)
      
        for (var i=0;i<res.columns.length;i++){ // columns
          var elem = document.createElement("td");
          //here parse the value acording to the column
          const column = res.columns[i]
          
          var currentDataValue = res.controllers[x][i]
          var valueElement;
          var controllerUUID;
          if(column.toLowerCase().includes("uuid")){
            //once gets controller uuid save it
            controllerUUID = currentDataValue;
            uuidInputElement.value = controllerUUID
          }
          if (column.includes("ownerUsername") && !currentDataValue){
            valueElement = createElementFromHTML(`<input type="submit" value="Attach" onclick="openAttachForm(this)">`)
          valueElement.appendChild(uuidInputElement)
            elem.appendChild(valueElement);
            row.appendChild(elem)
            continue; // exceptional write no data to cell because of the button
          }
          if (column.includes("date")){
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
          // append attach button
          elem.appendChild(valueElement);
          
          
          row.appendChild(elem)
        }
        
        container.appendChild(row)
      }

}
async function openAttachForm(controllerUUID){
  const model = document.getElementById("attachForm");
  model.open=true
  var usersRes = await getUsers()
  var usernameIdx=0;
  for(let i=0;i<usersRes.columns.length;++i){
    usernameIdx =usersRes.columns[i]==="username" ? i:usernameIdx;
  }
    
  const container = document.getElementById("attachControllerSelect")
  clearChildElements(container)
  for(let i=0;i<usersRes.users.length;++i){
    const tmp = document.createElement("option")
    tmp.value=usersRes.users[i][usernameIdx]
    tmp.innerText=usersRes.users[i][usernameIdx]
    container.appendChild(tmp);
  }
}
getClients();
