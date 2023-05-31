const reponseBoutonQ1 = document.getElementById("bouton01");
reponseBoutonQ1.addEventListener("click", function () {
  const boutonQ1 = document.getElementsByName("capitaleGuinee");
  let reponseGuineeCoche;
  for (let i = 0; i < boutonQ1.length; i++) {
    if (boutonQ1[i].checked) {
      reponseGuineeCoche = boutonQ1[i];
    }
  }
  if (reponseGuineeCoche.value === "conakry") {
    window.alert ("Félicitations voilà la bonne réponse !");
  } else {
    window.alert("GROS NUL ! Tu aurais dû écouter ta mère et aller à l'école");
  }
});

// --------------------------------------------------------------------------------
const reponseBoutonQ2 = document.getElementById("bouton02");
const reponse1 = document.getElementById("ue01");
const reponse2 = document.getElementById("ue02");
const reponse3 = document.getElementById("ue03");
const reponse4 = document.getElementById("ue04");

reponseBoutonQ2.addEventListener("click", function(){
	if ((reponse1.checked && reponse3.checked) && (!reponse2.checked && !reponse4.checked)){
      window.alert("Félicitations voilà la bonne réponse !");
  }else{
	      window.alert("GROS NUL ! Tu aurais VRAIMENT dû écouter ta mère et aller à l'école !!!!");
  }
})

