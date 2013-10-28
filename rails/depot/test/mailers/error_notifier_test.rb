require 'test_helper'

class ErrorNotifierTest < ActionMailer::TestCase
  test "report_error" do
    mail = ErrorNotifier.report_error
    assert_equal "Report error", mail.subject
    assert_equal ["to@example.org"], mail.to
    assert_equal ["from@example.com"], mail.from
    assert_match "Hi", mail.body.encoded
  end

end
