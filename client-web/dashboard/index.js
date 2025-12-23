async function logout(){
    const res = (await fetch(API_URL+"/logout",{
        method: "POST",
          headers: {}
    })).json();
     switch (res.code) {
          case 1: //error
            alert("error logging out")
            break;
          case 0: // success
            document.location = "/"
            break;
        }
}