async function logout(){
    var res = await fetch(API_URL+"/logout",{
        method: "POST",
          headers: {}
    });
    res = await res.json()
     switch (res.code) {
          case 1: //error
            alert("error logging out")
            break;
          case 0: // success
            document.location = "/"
            break;
          default:
            alert("error logging out: unexpected code")
        }
}

window.onload = function() {
  const container = document.getElementById("container")
  switch(getCurrentHas()){
    case '':{
      container.
    }
    case 
  }
   
}