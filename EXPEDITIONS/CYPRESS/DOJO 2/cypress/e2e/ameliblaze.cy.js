import { faker, th } from '@faker-js/faker';
describe('Demoblaze Dojo 02', () => {
// Création d'un utilisateur
  it('Création utilisateur', () => {
    // constantes faker
    const lastName = faker.person.lastName();
    const password = faker.internet.password();
    const country = faker.location.country();
    const city = faker.location.city();
    const creditCardNumber = faker.finance.creditCardNumber();
    cy.visit('https://www.demoblaze.com/');
    // Création d'un utilisateur
    cy.get('#signin2').click({ force: true });
    //cy.wait(500)
    cy.get('#sign-username').type(lastName);
    //cy.wait(500)
    cy.get('#sign-password').type(password);
    //cy.wait(500)
    cy.get('#signInModal > .modal-dialog > .modal-content > .modal-footer > .btn-primary').click({ force: true })
    //cy.get('#signInModal > .modal-dialog > .modal-content > .modal-footer > .btn-secondary').click({ force: true })
    // connexion
    //cy.wait(500)
    cy.get('#login2').click({ force: true })
    //cy.wait(500)
    cy.get('#loginusername', {timeout: 4000}).type(lastName)
    //cy.wait(500)
    cy.get('#loginpassword', {timeout: 4000}).type(password)
    //cy.wait(500)
    cy.get('#logInModal > .modal-dialog > .modal-content > .modal-footer > .btn-primary').click({ force: true })
    // ajout produit
    cy.wait(1000)
    cy.get(':nth-child(6) > .card > .card-block > .card-title > .hrefch').click({ force: true })
    cy.wait(1000)
    cy.get('.col-sm-12 > .btn').click({ force: true })
    cy.wait(1000)
    // confirmer commande {force true}
    cy.get('#cartur').click({ force: true })
    cy.get('.col-lg-1 > .btn').click({ force: true })
      //remplir formulaire
      cy.get('#name').type(lastName)
      cy.get('#country').type(country)
      cy.get('#city').type(city)
      cy.get('#card').type(creditCardNumber)
      cy.get('#month').type('05')
      cy.get('#year').type('2025')
      cy.get('#orderModal > .modal-dialog > .modal-content > .modal-footer > .btn-primary').click()
      // c'est tout joli d'avoir réussi
      cy.screenshot("validation-achat")
      // confirmation
      cy.get('.confirm').click()
  })
})