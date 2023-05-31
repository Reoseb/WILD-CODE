import { faker } from '@faker-js/faker'
describe('template spec', () => {
  it('passes', () => {
    const lastName = faker.person.lastName();
    const password = faker.internet.password();
    cy.visit('https://www.demoblaze.com/')
    cy.get('#signin2').click({force: true})
    cy.wait(500)
    cy.get('#sign-username').type(lastName)
    cy.get('#sign-password').type(password)
    cy.get('#signInModal > .modal-dialog > .modal-content > .modal-footer > .btn-primary').click({force: true})
    cy.get('#login2').click({force: true})
    cy.wait(500)
    cy.get('#loginusername').type(lastName)
    cy.get('#loginpassword').type(password)
    cy.get("#logInModal > .modal-dialog > .modal-content > .modal-footer > .btn-primary").click({force: true})
  })
})