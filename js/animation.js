const contacts = [
  "Chris:2232322",
  "Sarah:3453456",
  "Bill:7654322",
  "Mary:9998769",
  "Dianne:9384975",
];

const inp  = document.getElementById('search');
const p = document.querySelector('p');
const btn = document.querySelector('button');


btn.addEventListener('click', search);


function search(){
    // getting the value from the input element    
    const inputValue = inp.value.toLowerCase();
    inp.value = "";
    p.textContent='';
    inp.focus();
    // loop through the contacts and derive the names
    for(let i = 0 ; i < contacts.length; i++){
       people_contacts = contacts[i]
       .split(':')       
    //    getting the name element 
       names= people_contacts[0].charAt(0).toLowerCase()+ people_contacts[0].slice(1);
   

    //    getting the number elements 
        numbers = people_contacts[1];

    //    starting the comparator 
    if(names === inputValue){
        p.innerHTML = `the number of ${names} is ${numbers}`
        break;
    }
    else{
        p.innerHTML = `maybe an invalid input?`
    }
    
    console.log(names);
}

}




