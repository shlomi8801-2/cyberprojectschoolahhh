async function getCommands(){
    const maxrows=100;
    const offset = 0;
  var res = await fetch(API_URL + `/list/controllers/${maxrows}/${offset}`, {
    method: "get",
    headers: {"Token":getCookie("token")},
    credentials: 'include' // not working for some reason may fix later for now using in header
  });
  res = await res.json();
  console.log(res)
  return res
}
editMode = false
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
getCommands()