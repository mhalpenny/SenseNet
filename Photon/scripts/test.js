autowatch = 1;

var num = 10;

post(num);

function connect(){
  num++;
  post(num);
  post();
  outlet(0, num);
}
