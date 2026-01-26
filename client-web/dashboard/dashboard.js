editMode = false
myControllers = []

async function getMyControllers(){
    //get my controllers then get commands based on selected controller
    const maxrows=100;
    const offset = 0;
  var res = await fetch(API_URL + `/list/controllers/${maxrows}/${offset}`, {
    method: "get",
    headers: {"Token":getCookie("token")},
    credentials: 'include' // not working for some reason may fix later for now using in header
  });
  res = await res.json();
  myControllers = res;
  return res
}
async function getCommands(){

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
    //then submit to the server
  }else {
  //send the command id to the server
  }
}

getCommands()