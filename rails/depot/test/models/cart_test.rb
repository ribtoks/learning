require 'test_helper'

class CartTest < ActiveSupport::TestCase
  def setup
    @cart = Cart.create
    @book_one = products(:ruby)
    @book_two = products(:two)
  end

  test "add different products" do
    @cart.add_product(@book_one.id).save!
    @cart.add_product(@book_two.id).save!

    assert_equal 2, @cart.line_items.size
    assert_equal @book_one.price + @book_two.price, @cart.total_price
  end

  test "add same product" do
    number = 10

    number.times do
      @cart.add_product(@book_one.id).save!
    end

    assert_equal 1, @cart.line_items.size
    assert_equal number, @cart.line_items.first.quantity
    assert_equal @book_one.price * number, @cart.total_price
  end
end
