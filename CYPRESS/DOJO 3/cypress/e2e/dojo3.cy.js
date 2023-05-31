import { faker } from '@faker-js/faker';
before('faker', () =>{
      // consantes faker
      const lastName = faker.name.lastName();
      const firstName = faker.name.firstName();
      const email = faker.internet.email();
})
describe('testMagento', () => {
  it('Acceder aux tops', () => {
    cy.visit("https://magento.softwaretestingboard.com/")
    // intercept url
    cy.intercept({
      url:'https://magento.softwaretestingboard.com/customer/section/load/*',
      method : 'GET',
  }).as('waitToAddCart');
    // sélectionne les tops
    cy.get('#ui-id-4 > :nth-child(2)').trigger('mouseover') // survol de l'onglet "woman"
    cy.get('#ui-id-9').click() // click sur "tops"
    cy.wait(500)
    // sélectionne le premier top
    cy.get(':nth-child(1) > .product-item-info > .photo > .product-image-container > .product-image-wrapper > .product-image-photo').click()
    cy.get('#option-label-size-143-item-166').click() //sélectionne la taille XS
    cy.get('#option-label-color-93-item-57').click() // sélectionne la couleur "rose"
    cy.get('#product-addtocart-button').click() // ajouter le produit au panier
    cy.wait('@waitToAddCart')
    // click sur le panier
    cy.get('.showcart').click()
    cy.wait(1000)
    cy.get(':nth-child(7) > .secondary > .action > span').click({"force": true}) // sélectionner "proceed to checkout"
    // changer la quantité
    //cy.intercept({
    //  url:'https://magento.softwaretestingboard.com/pub/static/version1678540400/frontend/Magento/luma/en_US/*',
    //  method : 'GET',
  //}).as('waitToQtty');*/
    cy.wait(4000)
    cy.get('[id$="-qty"]').clear({force: true}).type('2', {force: true});
    //    cy.wait('@waitToQtty')
    cy.get('.update > span').click({"force": true})
    cy.get('.checkout-methods-items > :nth-child(1) > .action > span').click()
  // remplir formulaire
  })
})