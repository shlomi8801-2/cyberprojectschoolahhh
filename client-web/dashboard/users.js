async function getUsers() {
    const maxrows=100;
    const offset = 0;
  var res = await fetch(API_URL + `/list/${maxrows}/${offset}`, {
    method: "get",
    headers: {},
  });
  res = await res.json();
  switch (res.code) {
    case 1: //error
      alert("error gettings users list");
      console.error(res.error)
      break;
    case 0: // success
      //put them in the table
      console.log(res.users)
      break;
    default:
      alert("error getting users list: unexpected code");
  }
}