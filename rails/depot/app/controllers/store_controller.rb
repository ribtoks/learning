class StoreController < ApplicationController
  def index
    @products = Product.order(:title)

    @counter = increment_counter

    @display_counter = @counter > 5
  end

  private

  def increment_counter
    counter = session[:index_counter] || -1
    counter += 1
    
    session[:index_counter] = counter
    counter
  end
end
