class ErrorNotifier < ActionMailer::Base
  default from: "depot@example.com"

  # Subject can be set in your I18n file at config/locales/en.yml
  # with the following lookup:
  #
  #   en.error_notifier.report_error.subject
  #
  def report_error(error_message)
    @error_message = error_message
    
    mail to: "admin@example.com", subject: 'Internal error'
  end
end
