editMode = false
myControllers = []
currentCommands = {"controllerId":0,"commands":[],"columns":[]}
class controllerClass{
  constructor(data,columns) {
  
  this.uuid = data[columns.indexOf("uuid")]
  this.OwnerUsername =data[columns.indexOf("ownerUsername")]
  this.availablePins= data[columns.indexOf("availablePins")].split("").map((x) => {x.charCodeAt(0)})
  }
 
}
selectedController = null

async function makeFetch(url,additionalHeaders={},method="get",body={}){
  const res = await fetch(url,{
    headers: {"Token":getCookie("token"),...additionalHeaders},
    method: method,
    body:(method=="post" ?body:null)
  });
  return await res.json()
}
async function fetchMyControllers(){
    //get my controllers then get commands based on selected controller
    const maxrows=100;
    const offset = 0;
  myControllers = await makeFetch(API_URL + `/list/myControllers/${maxrows}/${offset}`);
  return myControllers
}
async function fetchCommands(controllerId){
    //get commands from the server based on controllerId(the server checks if it yours)
    const maxrows=100;
    const offset = 0;
  return await makeFetch(API_URL + `/list/commands/${maxrows}/${offset}`,{"uuid":controllerId},"get")
}
function getRowByColumn(columns,iterable,columnName, value){
  const rows =iterable.filter((row)=>row[columns.index(columnName)] == value);
  return rows.length==1 ? rows[0]:null;
}
function getCommandRow(commandTitle){
  //gets a command tuple from currentCommands by commandTitle
  return getRowByColumn(currentCommands.commands.columns,currentCommands.commands.commands,"title",commandTitle);
}
function editBtnClick(){
  const editBtn = document.getElementById("editBtn");
  if (editMode){
    //its confirm
    editBtn.querySelector("div#face0").style.display = "block"
    editBtn.querySelector("div#face1").style.display = "none"
    submitChanges();
  }else if (!editMode){
    //enter edit
    editBtn.querySelector("div#face0").style.display = "none"
    editBtn.querySelector("div#face1").style.display = "block"
  }
  editMode = !editMode
}


function getBtnId(htmlBtn){
  //get hidden input with id "BTNID" and get its value
  //https://stackoverflow.com/questions/2899072/get-child-by-id
  const inputElem = htmlBtn.querySelector("input#BTNID");
  return inputElem ? inputElem.value : null;
}

function btnClick(thisBtn){
  btnUUID =getBtnId(thisBtn)
  if (editMode){
    const editForm = document.getElementById("editform");
    editForm.style.display="block"
    console.log(myControllers.controllers)
    thisBtnInfo = myControllers.controllers.filter((row)=>row[myControllers.columns.index("ID")]==btnUUID)
    thisBtnInfo = thisBtnInfo.length >= 1 ?thisBtnInfo[0] : null
    //here add the columns to the editform
    //then submit to the server - in submitChanges function
   
    
  }else {
  //send the command id to the server
  makeFetch(API_URL + `/controllers/${selectedController.uuid}/execute`,{},"post")
  }
}
function submitChanges(){
  const editForm = document.getElementById("editform");
  if (editForm.style.display == "none"){
    //the form is not shown(caused by the confirm edit button) then just skip because there is no pneding changes
    return

  }
  setTimeout(function(){},1000); // delay before closing the form
  editForm.style.display = "none"
}
function displayButtons(){
  //take from currentCommands.commands
  const btnGrid = document.getElementById("btnGrid")
  const titleIndex = currentCommands.columns.index("title")
  const btnIdIndex = currentCommands.columns.index("title") // for now
  //       <button type="button" class="commandButton" onclick="btnClick(this)">test button1 <input type="hidden" value="id" id="BTNID"></button>
  while (btnGrid.firstChild) { // clear childs in btnGrid
        btnGrid.removeChild(btnGrid.firstChild);
    }
  for (var i=0;i<currentCommands.commands.length;++i){
    var btn = document.createElement("button")
    var hiddenBtnInput = document.createElement("input")
    btn.class="commandButton"
    btn.onclick="btnClick(this)"
    btn.innerText=currentCommands.commands[i][titleIndex]
    hiddenBtnInput.type="hidden"
    hiddenBtnInput.value=currentCommands.commands[i][btnIdIndex]
    btn.appendChild(hiddenBtnInput)
    btnGrid.appendChild(btn)
  }
}
function setupEditForm(){
  const editForm = document.getElementById("editform");
  const submitBtn = `<button type="button" onclick="submitChanges()">submit</button>`
  while (btnGrid.firstChild) { // clear childs in editForm
        btnGrid.removeChild(btnGrid.firstChild);
  }
  if (currentCommands.columns)
  for (var i=0;i<currentCommands.columns;++i){
    var inputElem = document.createElement("label")
    inputElem.innerText=currentCommands.columns[i]
    editForm.appendChild(inputElem)
    inputElem = document.createElement("input")
    inputElem.type="text"
    inputElem.id=currentCommands.columns[i]
  }
  editForm.appendChild(submitBtn)

}

async function startup(){
//fetch everything needed and display
  await fetchMyControllers()
  
  if(myControllers.controllers && myControllers.controllers.length ==0){
    // user has no controllers
    return
  }

  selectedController= new controllerClass(myControllers.controllers[0],myControllers.columns)
  await fetchCommands(selectedController.uuid)
  if (!currentCommands.columns){
    return
  }
  setupEditForm()
  displayButtons()
  
}
startup()