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
function changewindow(){
const container = document.getElementById("container")
  switch(getCurrentHash()){
    case "#clients":{
container.src = "clients.html"
      break;
    }
    case "#users":{
container.src = "users.html"
      break;
    }
    case "#about":{
container.src = "about.html"
      break;
    }
    default:{
      container.src = "dashboard.html"
      break;
    }
  }
   
}
navigation.addEventListener("navigate", e => {
changewindow()
});

