# works with both python 2 and 3
from __future__ import print_function

import africastalking

class SMS:
    def __init__(self):
		# Set your app credentials
        self.api_key = "450932204ce6cf1cd1de357dc12e0f468d060b34c099b4d82f6d5428924e46de"
        self.username = "franklin_e"
        africastalking.initialize(self.username, self.api_key)
        self.sms = africastalking.SMS 

    def send(self):
            # Set the numbers you want to send to in international format
            recipients = ["+254789547024", "+25492873281"]

            # Set your message
            message = "my name is frankline and I ` like gaming "

            # Set your shortCode or senderId

            sender = "shortCode or senderId"
            try:
				# Thats it, hit send and we'll take care of the rest.
                response = self.sms.send(message, recipients, sender)
                print (response)
            except Exception as e:
                print ('Encountered an error while sending: %s' % str(e))

if __name__ == '__main__':
    SMS().send()