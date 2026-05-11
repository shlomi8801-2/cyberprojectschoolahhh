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
    body:(method.toLowerCase()==="post" ?body:null)
  });
  return await res.json()
}
async function fetchMyControllers(){
    //get my controllers then get commands based on selected controller
    const maxrows=100;
    const offset = 0;
  myControllers = await makeFetch(API_URL + `/list/myControllers/${maxrows}/${offset}`);
  if (myControllers.code===1){
    alert("error fetching myControllers: "+myControllers.error);
    return []
  }
  return myControllers
}
async function fetchCommands(controllerId){
    //get commands from the server based on controllerId(the server checks if it yours)
    const maxrows=100;
    const offset = 0;
  return await makeFetch(API_URL + `/list/commands/${maxrows}/${offset}`,{"uuid":controllerId},"get")
}
function getRowByColumn(columns,iterable,columnName, value){
  const rows =iterable.filter((row)=>row[columns.indexOf(columnName)] == value);
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
  }else if (!editMode){
    //enter edit
    editBtn.querySelector("div#face0").style.display = "none"
    editBtn.querySelector("div#face1").style.display = "block"
  }
  editMode = !editMode
}
async function addCommandBtnClick(){
  //send setupEditForm with blank data so it will add
  setupEditForm(["","",""])
}
async function addCommand(){
  const formData = getDataFromEditForm()
  formData.ControllerId = selectedController.uuid
  const res = await makeFetch(API_URL + `/controllers/${selectedController.uuid}/add`,{"content-type":"application/json"},"POST",JSON.stringify(formData))
  if(res.code===0){//refresh all buttons
    currentCommands = await fetchCommands(selectedController.uuid)
    displayButtons()
  }
}

function getBtnId(htmlBtn){
  //get hidden input with id "BTNID" and get its value
  //https://stackoverflow.com/questions/2899072/get-child-by-id
  const inputElem = htmlBtn.querySelector("input#BTNID");
  return inputElem ? inputElem.value : null;
}
function getCommandByTitle(title){
  let i=0;
  for(i=0;i<currentCommands.commands.length;++i){
    if (currentCommands.commands[i][currentCommands.columns.indexOf("title")]===title)
      break;
  }
  return currentCommands.commands[i];
}

function btnClick(thisBtn){
  const btnUUID =getBtnId(thisBtn)
  if (editMode){
    const editForm = document.getElementById("editform");
    thisBtnInfo = myControllers.controllers.filter((row)=>row[myControllers.columns.indexOf("uuid")]==btnUUID)
    thisBtnInfo = thisBtnInfo.length >= 1 ?thisBtnInfo[0] : null
    const btnData = getCommandByTitle(btnUUID);
    setupEditForm(btnData)
  }else {
  //send the command id to the server
  makeFetch(API_URL + `/controllers/${selectedController.uuid}/execute`,{},"post")
  }
}
function getDataFromEditForm(){
    const editForm = document.getElementById("editform");
    //the edit form is built in a loop so read it like it
    const output = {}
    editForm.childNodes.forEach(Node => {
      if (Node.tagName==="INPUT"){
        output[Node.name] = Node.value
      }
    });
    return output 
}
async function submitChanges(btnTitle){
  //run on submit in edit form
  const editForm = document.getElementById("editform");
  const res = await makeFetch(API_URL + `/controllers/${selectedController.uuid}/update`,{"content-type":"application/json","OldTitle":btnTitle},"POST",JSON.stringify(getDataFromEditForm()))
  console.log(res)
  if(res.code===0){//refresh all buttons
    currentCommands = await fetchCommands(selectedController.uuid)
    displayButtons()
  }

  
}
function displayButtons(){
  //take from currentCommands.commands
  const btnGrid = document.getElementById("btnGrid")
  const titleIndex = currentCommands.columns.indexOf("title")
  const btnIdIndex = currentCommands.columns.indexOf("title") // for now
  //       <button type="button" class="commandButton" onclick="btnClick(this)">test button1 <input type="hidden" value="id" id="BTNID"></button>
  clearChildElements(btnGrid)
  for (var i=0;i<currentCommands.commands.length;++i){
    var btn = createElementFromHTML(`      <button type="button" class="commandButton" onclick="btnClick(this)">test button</button>`)
    var hiddenBtnInput = document.createElement("input")
    btn.innerText=currentCommands.commands[i][titleIndex]
    hiddenBtnInput.type="hidden"
    hiddenBtnInput.id = "BTNID"
    hiddenBtnInput.value=currentCommands.commands[i][btnIdIndex]
    btn.appendChild(hiddenBtnInput)
    btnGrid.appendChild(btn)
  }
}

function setupEditForm(btnData){
  //run everytime needs to open the edit menu
  const editForm = document.getElementById("editform");
  var submitBtn;
  if (btnData.length ===currentCommands.columns.length && btnData[0].length>=1){ // if full of data means all editing at least for now
    submitBtn = createElementFromHTML(`<button type="button" onclick="{submitChanges('${btnData[currentCommands.columns.indexOf("title")]}'); this.parentNode.style.display = 'none';}">submit</button>`)
  }else{
    submitBtn = createElementFromHTML(`<button type="button" onclick="{addCommand(); this.parentNode.style.display = 'none';}">submit</button>`)
  }
  
  clearChildElements(editForm)
  if (currentCommands.columns){
      for (var i=0;i<currentCommands.columns.length;++i){
        var inputElem = document.createElement("label");
        inputElem.innerText=currentCommands.columns[i];
        editForm.appendChild(inputElem)
        inputElem = document.createElement("input")
        inputElem.type="text"
        inputElem.name=currentCommands.columns[i]
        inputElem.value = btnData[i];
//disable the unchangeable value(id)
    if (currentCommands.columns[i] === "ControllerId")
      inputElem.disabled ="disabled";
    editForm.appendChild(inputElem)
  }
  }
  editForm.appendChild(submitBtn)
  editForm.style.display="block" //show the form
}

async function startup(){
//fetch everything needed and display
  await fetchMyControllers()
  
  if(myControllers.controllers && myControllers.controllers.length ==0){
    // user has no controllers
    return
  }
  currentCommands.code=1 //set failed status for current commands
  for(let i=0;i<myControllers.controllers.length && currentCommands.code===1;++i) { // if failed to get commands for a controller try each one - for now there is no way of choosing controller in the user interface
    selectedController= new controllerClass(myControllers.controllers[i],myControllers.columns)
  currentCommands = await fetchCommands(selectedController.uuid)
  }
  
  displayButtons()
  
}
startup()