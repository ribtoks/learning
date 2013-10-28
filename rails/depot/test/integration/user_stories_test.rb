require 'test_helper'

class UserStoriesTest < ActionDispatch::IntegrationTest
  fixtures :products

  def setup
    LineItem.delete_all
    Order.delete_all
    @ruby_book = products(:ruby)
  end

  # A user goes to the index page. They select a product, adding it to their
  # cart, and check out, filling in their details on the checkout form. When
  # they submit, an order is created containing their information, along with a
  # single line item corresponding to the product they added to their cart.

  test "buying a product" do
    
    customer_name = "Dave Thomas"
    customer_address = "123 The Street"
    customer_email = "dave@example.com"
    customer_pay_type = "Check"

    # user loads a page
    get "/"
    assert_response :success
    assert_template "index"

    # selects a product, adds to cart

    xml_http_request :post, '/line_items', product_id: @ruby_book.id
    assert_response :success

    cart = Cart.find(session[:cart_id])
    assert_equal 1, cart.line_items.size
    assert_equal @ruby_book, cart.line_items.first.product

    # checkout

    get "/orders/new"
    assert_response :success
    assert_template "new"

    # submit user details

    post_via_redirect("/orders",
                      order: {
                        name: customer_name,
                        email: customer_email,
                        address: customer_address,
                        pay_type: customer_pay_type})

    assert_response :success
    assert_template "index"
    cart = Cart.find(session[:cart_id])
    assert_equal 0, cart.line_items.size

    # ensure order is created

    orders = Order.all
    assert_equal 1, orders.size
    order = orders.first

    assert_equal customer_name, order.name
    assert_equal customer_email, order.email
    assert_equal customer_address, order.address
    assert_equal customer_pay_type, order.pay_type

    assert_equal 1, order.line_items.size
    line_item = order.line_items[0]
    assert_equal @ruby_book, line_item.product

    # check email

    mail = ActionMailer::Base.deliveries.last
    assert_equal [customer_email], mail.to
    assert_equal 'Name Surname <depot@example.com>', mail[:from].value
    assert_equal 'Order Confirmation', mail.subject    
  end  
end
