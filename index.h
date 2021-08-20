const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>Controle de Tempo</title>
    <style>
        h2  {
  text-align: center;
}
table {
  font-family: arial, sans-serif;
  border-collapse: collapse;
  width: 100%;
}

td, th {
  border: 1px solid #dddddd;
  text-align: center;
  padding: 8px;
}

tr:nth-child(even) {
  background-color: #dddddd;
}

.forms{
display:flex;
flex-direction: row;
justify-content: space-between;
align-items: center;
}

.close {
  cursor: pointer;
  top: 50%;
  right: 0%;
  padding: 12px 16px;
  transform: translate(0%, -50%);
}

.close:hover {background: #bbb;}

input[type=text] , text{
  
  background-color: white;
  background-image: url('searchicon.png');
  background-position: 10px 10px;
  background-repeat: no-repeat;
  padding-left: 40px;
}

.form{
  font-size:larger ;
}

.add{
  background-color: #4CAF50;
  border: none;
  color: white;
  padding: 16px 32px;
  text-decoration: none;
  margin: 4px 2px;
  cursor: pointer;
}
.del{
  background-color: #FF0000;
  border: none;
  color: white;
  padding: 16px 32px;
  text-decoration: none;
  margin: 4px 2px;
  cursor: pointer;
}
.export{
  background-color: #252FFC;
  border: none;
  color: white;
  padding: 16px 32px;
  text-decoration: none;
  margin: 4px 2px;
  cursor: pointer;
}

</style>
    
</head>
<body>
  <h2>
      Tabela de Tempos
    </h2>
<div class="forms">
<input class="add" type="button" value="Adicionar tempo" onclick="addRow()" id="add">
<div class="form">
Nome:
<input type="text" name="name" id="name" /><br />
Tempo:
<text type="text" name="time" id="time" value = "0" /><br />
</div>
<input class="export" type="button" value="Exportar Dados" onclick="exportCsv()" id="add">
<input class="del" type="button" value="Deletar Dados" onclick="delDados()" id="add">
</div>
<table id="table" border="1">
    <tr>
        <th>Nome</th>
        <th>Tempo</th>
    </tr>
</table>
    
    
</body>

<script>
  getJson();
setInterval(getTime, 3000);
var time = "0";
var rcJson = [];

function getTime(){
document.getElementById("time").innerHTML = time ;

var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("time").innerHTML =
      (this.responseText + " Segundos");
    console.log(this.responseText);
    }
  };
  xhttp.open("GET", "getTime", true);
  xhttp.send();

}

const toCsv = function(table) {
    // Query all rows
    const rows = table.querySelectorAll('tr');

    return [].slice.call(rows)
        .map(function(row) {
            // Query all cells
            const cells = row.querySelectorAll('th,td');
            return [].slice.call(cells)
                .map(function(cell) {
                    return cell.textContent;
                })
                .join(',');
        })
        .join('\n');
};

const download = function(text, fileName) {
    const link = document.createElement('a');
    link.setAttribute('href', `data:text/csv;charset=utf-8,${encodeURIComponent(text)}`);
    link.setAttribute('download', fileName);

    link.style.display = 'none';
    document.body.appendChild(link);

    link.click();

    document.body.removeChild(link);
};

function exportCsv(){
const table = document.getElementById('table');
  const csv = toCsv(table);
    download(csv, 'Corrida.csv');
}

function addRow() {
   "use strict";
    
    var table = document.getElementById("table");
    var time = document.getElementById("time").innerHTML;
   var row = table.insertRow(1);
   var td1 = row.insertCell(0);
   var td2 = row.insertCell(1);;   

    td1.innerHTML = document.getElementById("name").value;
    td2.innerHTML  = time;
  
  var xhttp = new XMLHttpRequest();

  let newoObj = {Nome: document.getElementById("name").value, Tempo: time};
  console.log(rcJson);
  rcJson.unshift(newoObj);
  //console.log(data);
  xhttp.open("POST", "postJson");
  xhttp.send(JSON.stringify(rcJson));
};
  function delDados(){
    var xhttp = new XMLHttpRequest();
    xhttp.open("get", "delDados",true);
    xhttp.send();
    location.reload();
    }

  function getJson(){
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        console.log("getJson");
        
        rcJson = this.responseText;
        if(rcJson !== "vazio"){
          rcJson = JSON.parse(rcJson)
        console.log(rcJson);
        
     rcJson.forEach(x =>{
       var table = document.getElementById("table");
    
    var row= document.createElement("tr");
    //console.log(row);
    var td1 = document.createElement("td");
    var td2 = document.createElement("td");
    
    td1.innerHTML = x.Nome;
    td2.innerHTML  = x.Tempo;

    row.appendChild(td1);
    row.appendChild(td2);
    table.children[0].appendChild(row);
    })
    }else{
      rcJson = [];
      }
    }
  };
  xhttp.open("GET", "getJson", true);
  xhttp.send();
       }

      
  </script>
</html>
)=====";
