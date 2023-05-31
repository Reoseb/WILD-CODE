METHODE ALEX

const mangas = ["Vagabond","Dragon Ball Z","Kingdom","Albator","Fairy Tales"];
let films=[];
console.log (mangas[mangas.length-1]);


METHODE AMELIE

const mangas = ["Vagabond","Dragon Ball Z","Kingdom","Albator","Fairy Tales"];
function nouveautes(){
   return(mangas[mangas.length-1]);
}
 nouveautes (mangas);


let phraseA = "";
let phraseB = "";
function concatPhrase(phraseA,phraseB){
    return(phraseA + " | " + phraseB)
}
concatPhrase(Bonjour, mes petits amis);

//Exo 6

let phrase13 = "sdglgkjqsgln"
 let phrase23 = "pouet pouet"

 if (phrase13.length === phrase23.length) {
return true
 } else {
return false
 }

 Amélie
16:52
let phraseA = "";
let phraseB = "";
function concatPhrase(phraseA,phraseB){
    return(phraseA + " | " + phraseB)
}
concatPhrase(Bonjour, mes petits amis);
Alex Skaree
17:09
let phrase13 = "sdglgkjqsgln"
 let phrase23 = "pouet pouet"

 if (phrase13.length === phrase23.length) {
return true
 } else {
return false
 }
Alex Skaree
17:15
let phrase13 = "sdglgkjqsgln";
 let phrase23 = "pouet pouet";

 function comparePhrase (phrase13,phrase23){
 if (phrase13.length === phrase23.length) {
   return true;
 } else {
   return false;
 }}
 comparePhrase(phrase13,phrase23)
Amélie
17:16
let phraseA = "";
let phraseB = "";
function phrasePluGrande(phraseA, phraseB){
    if (phraseA.length == phraseB.length) {
        return true;
    }
    else{
        return false;
    }
}

phrasePluGrande("bonjour tout les monde", "comment ça va ?");

//exo 8
let nombre1;

function pairOuImpair(nombre1) {
  if (nombre1 % 2 == 0) {
    return "pair";
  } else {
    return "impair";
  }
}