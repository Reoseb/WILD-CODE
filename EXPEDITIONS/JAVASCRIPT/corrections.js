/**
 * Exercice 2 :
 */
console.log("/*** EXERCICE 2 :  **\\");

// 4! = 4 * 3 * 2 * 1 = 24
// 5! = 5 * 4 * 3 * 2 * 1 = 120

function factorielle(n) {
  let temp = 1;
  for (let i = 1; i <= n; i++) {
    temp = temp * i;
  }
  return temp;
}

console.log(factorielle(5));
console.log(factorielle(1));
console.log(factorielle(4));
console.log(factorielle(0));
console.log(factorielle(100));

/**
 * Exercice 3 :
 */
console.log("/*** EXERCICE 3 :  **\\");

function lePlusGrand(a, b, c) {
  // lex maxTemp = a > b ? a : b

  let maxTemp;
  if (a > b) {
    maxTemp = a;
  } else {
    maxTemp = b;
  }

  if (c > maxTemp) {
    maxTemp = c;
  }
  return maxTemp;
}

console.log(lePlusGrand(1, 2, 3));
console.log(lePlusGrand(2, 15, 0));
console.log(lePlusGrand(0, 0, 2));

/**
 * Exercice 4 :
 */
console.log("/*** EXERCICE 4 :  **\\");

function dernierElement(tab) {
  return tab[tab.length - 1];
}

const tab = [1, 2, 3, 5, 11];

console.log();
dernierElement([1, 2, 3, 5, 11]);
dernierElement(tab);

/**
 * Exercice 5 :
 */
console.log("/*** EXERCICE 5 :  **\\");

function concatStrings(str1, str2) {
  // return str1 + ' | ' + str2
  return `${str1} | ${str2}`;
}

/**
 * Exercice 6 :
 */

console.log("/*** EXERCICE 6 :  **\\");

function checkSameLength(str1, str2) {
  return str1.length === str2.length;

  /*     if(str1.length === str2.length) {
        return true
    } else {
        return false
    } */
}

/**
 * Exercice 7 :
 */

console.log("/*** EXERCICE 7 :  **\\");

function checkPair(n) {
  if (n % 2 === 0) {
    return "pair";
  } else {
    return "impair";
  }
}

/**
 * Exercice 8 :
 */

console.log("/*** EXERCICE 8 :  **\\");

function capital(country) {
  let capitalCity = "";
  switch (country) {
    case "France":
      city = "Paris";
      break;
    case "Allemagne":
      city = "Berlin";
      break;
    case "Italie":
      city = "Rome";
      break;
    case "Maroc":
      city = "Rabat";
    case "Espagne":
      city = "Madrid";
      break;
    case "Portugal":
      city = "Lisbonne";
      break;
    case "Angleterre":
      city = "Londres";
      break;
    default:
      console.log(
        `Désolé je ne suis pas dans la possibilité de fournir la capital de ${country}`
      );
  }
}

/**
 * Exercice 9 :
 */

console.log("/*** EXERCICE 9 :  **\\");

function calculatrice() {
  const a = parseFloat(prompt("Veuillez entrer le premier chiffre"));
  const operation = prompt("Veuillez entrer l'operation souhaitée");
  const b = parseFloat(prompt("Veuillez entrer le deuxième chiffre"));
  let resultat;
  switch (operation) {
    case "+":
      resultat = a + b;
      break;
    case "-":
      resultat = a - b;
      break;
    case "*":
      resultat = a * b;
      break;
    case "/":
      resultat = (b !== 0) ? a / b : "Ne peut diviser par 0";
      if( b!== 0) {
        resultat = a / b
      } else {
        resultat = "Ne peut diviser par 0"
      }
      break;
    default:
      resultat = "Operation inconue";
      break;
  }

  console.log(resultat);
}
calculatrice();
