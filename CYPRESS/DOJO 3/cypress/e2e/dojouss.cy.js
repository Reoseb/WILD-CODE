import { faker, th } from '@faker-js/faker';

describe('testMagento', () => {
  
  it('Accéder aux tops', () => {

    

    const lastName = faker.person.lastName();
    const firstName = faker.person.firstName();
    const email = faker.internet.email();
   
    cy.intercept({
        url:"https://magento.softwaretestingboard.com/customer/section/load/*",
        method:"GET",
    }).as('waitAddToCart');
    cy.intercept({
        url:"https://magento.softwaretestingboard.com/pub/static/version1678540400/frontend/Magento/luma/en_US/Magento_CheckoutAgreements/template/checkout/checkout-agreements.html",
        method:"GET",
    }).as('waitPayment');
    cy.intercept({
        url:"https://magento.softwaretestingboard.com/pub/static/version1678540400/frontend/Magento/luma/en_US/Magento_Checkout/template/progress-bar.html",
        method:"GET",
    }).as('waitShipping');
    cy.visit('https://magento.softwaretestingboard.com')

    cy.get('#ui-id-4 > .ui-menu-icon').trigger('mouseover')
    cy.get('#ui-id-9').click()
    cy.get(':nth-child(1) > .product-item-info > .photo > .product-image-container > .product-image-wrapper > .product-image-photo').click()
    cy.get('#option-label-size-143-item-167').click()
    cy.get('#option-label-color-93-item-57').click()
    cy.get('#product-addtocart-button').click()
    cy.wait("@waitAddToCart")
    cy.get('.showcart').click()
    cy.wait(1000)
    cy.get('[id^="cart-item-"][id$="-qty"]').clear().type(3)
    cy.get('[id^="update-cart-item-"]').click()
    cy.wait("@waitAddToCart")
    cy.get('#top-cart-btn-checkout').click()
    cy.wait("@waitShipping")

    
  })
})
/*
<input class="input-text" type="text" data-bind="
    value: value,
    valueUpdate: 'keyup',
    hasFocus: focused,
    attr: {
        name: inputName,
        placeholder: placeholder,
        'aria-describedby': getDescriptionId(),
        'aria-required': required,
        'aria-invalid': error() ? true : 'false',
        id: uid,
        disabled: disabled
    }" name="firstname" aria-required="true" aria-invalid="false" id="FWGUYGH">
    */