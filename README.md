# cyberprojectschoolahhh

## had to put it on public enjoy this yud bet(israeli yes) project

#### system specs:

* server: whatever can run this and not break(like python 3.12)
* client: browser + external board which you can tweak with my code to work with for example i used arduino uno with `sim7000G` module i found on [aliexpress](https://he.aliexpress.com/item/1005009498393714.html?algo_exp_id=98b135d9-4139-42cb-a93a-1e6131ddac70-46&pdp_ext_f=%7B%22order%22%3A%221%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005009498393714%7C_p_origin_prod%3A&_gl=1\*1whfkev\*_gcl_au\*ODI1NzU0MjI4LjE3NjYyNDczMzc.\*_ga\*MTQ1Mjk5Nzk0My4xNzU1Njc4NzY2\*_ga_VED1YSGNC7\*czE3NjYzMzc1MTckbzYkZzEkdDE3NjYzMzc3MDEkajU1JGwwJGgw)


<details>
<summary>things i used from the internet for this project</summary>

* [pico css](https://github.com/picocss/pico)
* [w3schools](https://www.w3schools.com/)
* [tinygsm](https://github.com/vshymanskyy/TinyGSM)
* [css loaders](https://css-loaders.com)

  </details>

### how to run?

#### for self hosting your-self

requirements:

* \`\`python 3.10\`\`

first copy this repo to some sort of a computer/server
run `bash  python3 ./server/server.py`
and then host the `client-web` directory somewhere can be on nginx root apache2 or python3 by running the command in `client-web/run.sh` file to host the current directory

#### to setup client(for self hosted also)

compile the client.c file in the root of the repo
upload it to your device(arduino for example)
and if you are using your own server change the server addresses in the client first lines in the `#define`'s