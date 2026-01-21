async function getCommands(){
    const maxrows=100;
    const offset = 0;
  var res = await fetch(API_URL + `/list/controllers/${maxrows}/${offset}`, {
    method: "get",
    headers: {"Token":getCookie("token")},
    credentials: 'include' // not working for some reason may fix later for now using in header
  });
  res = await res.json();
  // console.log(res)
  return res
}
function editBtnClick(){
  const editBtn = document.getElementById("editBtn");
  if (editMode){
    //its confirm
    editBtn.getElementById("face0").style.display = "block"
    editBtn.getElementById("face1").style.display = "none"
  }else if (!editMode){
    //enter edit
    editBtn.getElementById("face0").style.display = "none"
    editBtn.getElementById("face1").style.display = "block"
  }
  editMode = !editMode
}

function getBtnId(htmlBtn){
  //get hidden input with id "BTNID" and get its value
  //https://stackoverflow.com/questions/2899072/get-child-by-id
  const inputElem = htmlBtn.querySelector("input#BTNID");
  return inputElem ? inputElem.value : null;
}

editMode = false
getCommands()