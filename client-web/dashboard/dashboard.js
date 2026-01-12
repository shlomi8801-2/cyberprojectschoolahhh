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
getCommands()