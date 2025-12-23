    async function login() {
      try {
        const username = document.getElementById("usernameBox").value;
        const password = document.getElementById("passwordBox").value;
        // console.log(`username:${username} password: ${password}`);
        if (!API_URL) {
          alert("failed to acuire  API_URL!");
          return;
        }
        res = await fetch(API_URL + "/login", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify({ username: username, pass: password }),
        });
        const statusBox = document.getElementById("status");
        res = await res.json();
        switch (res.code) {
          case 1: //error
            statusBox.innerText = res.error;
            statusBox.style.color = "var(--pico-form-element-invalid-active-border-color)";
            break;
          case 0: // success
            statusBox.style.color = "var(--pico-h2-color)";
            statusBox.innerText = "redirecting";
            setCookie("token", res.token);
            document.location = "../dashboard"
            //now redirect to wherever needed
            //...
            break;
        }
      } catch (error) {
        const statusBox = document.getElementById("status");
        statusBox.innerText = `something went wrong!\n${error}`;
        console.error(error);
      }
    }
    async function register() {
      try {
        const username = document.getElementById("usernameBox").value;
        const password = document.getElementById("passwordBox").value;
        // console.log(`username:${username} password: ${password}`);
        if (!API_URL) {
          alert("failed to acuire  API_URL!");
          return;
        }
        res = await fetch(API_URL + "/register", {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify({
            username: username,
            pass: password,
            cpass: password,
          }),
        });
        res = await res.json()
        switch (res.code){
          case 1: 
          statusBox.innerText = res.error;
            statusBox.style.color = "var(--pico-form-element-invalid-active-border-color)";
            break;
          case 0:
            login();
        }
      } catch (error) {
        const statusBox = document.getElementById("status");
        statusBox.innerText = `something went wrong!\n${error}`;
        console.error(error);
      }
    }