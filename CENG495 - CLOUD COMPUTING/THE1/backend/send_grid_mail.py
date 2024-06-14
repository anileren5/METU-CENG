import ssl
from sendgrid import SendGridAPIClient
from sendgrid.helpers.mail import Mail

ssl._create_default_https_context = ssl._create_unverified_context

def sendMail(title, old_price, new_price,users_emails):
    message = Mail(
        from_email='anil.gocer@ceng.metu.edu.tr',
        to_emails=users_emails,
        subject='Cengden Price Decrease Notification',
        html_content=f'<div><p>Dear Cengden User,</p><p>This email is sent to notify you that price of the item with title <strong>{title}</strong> is decreased to <strong>{new_price}</strong> from <strong>{old_price}</strong>.<p></div>'
        )
    try:
        sg = SendGridAPIClient("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")
        response = sg.send(message)
        print(response.status_code)
        print(response.body)
        print(response.headers)
        return True
    except Exception as e:
        return False

