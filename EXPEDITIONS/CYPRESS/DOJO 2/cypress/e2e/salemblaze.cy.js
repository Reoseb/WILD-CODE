import { faker } from '@faker-js/faker'
describe('Testing E commerce”, () => {
  it('Création utilisateur, ajout de produit au panier, et passage de commande”, () => {
    // constantes faker
    const lastName = faker.name.lastName(),
    const password = faker.internet.password(),
    cy.visit('https://www.demoblaze.com/'),
    cy.screenshot('Page d’accueil”)
    // Création d’un utilisateur
    cy.get('#signin2").click(),
    cy.get('#sign-username”).type(lastName)
    cy.wait(500),
    cy.get('#sign-password”).type(password),
    cy.wait(500),
    cy.get(
      '#signInModal > .modal-dialog > .modal-content > .modal-footer > .btn-primary”
    ).click({ force: true }),
    cy.screenshot('Page de création utilisateur'),
    cy.get(
      '#signInModal > .modal-dialog > .modal-content > .modal-footer > .btn-secondary”
    ).click({ force: true }),
    // Connexion
    cy.get('#login2").click(),
    cy.get('#loginusername”).type(lastName),
    cy.get('#loginpassword”).type(password),
    cy.get(
      '#logInModal > .modal-dialog > .modal-content > .modal-footer > .btn-primary”
    ).click({ force: true }),
    cy.screenshot('Page de connexion”),
    // Navigation vers la page de produit
    cy.visit('https://www.demoblaze.com/prod.html?idp_=3”),
    cy.screenshot('Page de produit”),
    // Ajout du produit au panier
    cy.get('.col-sm-12 > .btn”).click({ force: true }),
    cy.screenshot('Page du panier”),
    // Aller au panier
    cy.visit('https://www.demoblaze.com/cart.html”),
    cy.screenshot('Page du panier'),
    // Passage de commande
    cy.get('.col-lg-1 > .btn”).click({ force: true }),
    cy.get('#name”).type(faker.name.firstName()),
    cy.get('#country”).type(faker.address.country()),
    cy.get('#city”).type(faker.address.city()),
    cy.get('#card”).type(faker.finance.creditCardNumber()),
    cy.get('#month”).type('05”),
    cy.get('#year”).type('2025”),
    cy.get(
      '#orderModal > .modal-dialog > .modal-content > .modal-footer > .btn-primary”
    ).click({ force: true }),
    cy.screenshot('Page de confirmation de commande”),
  }),
}),