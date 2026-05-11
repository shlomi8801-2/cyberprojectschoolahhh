
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







async function setup(){
  displayUsers(await getUsers())
}
setup()


