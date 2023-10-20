#!/usr/bin/python3

import cgi, cgit
import os

form = cgi.FieldStorage()
print("Content-Type: text/html\n")
print('<html>')
print('<head>')
print('<title>Capitalise</title>')
print('</head>')
print('<body>')

if os.environ['REQUEST_METHOD'] == 'GET':
	for form_key, form_value in form.items():
		print(f"{form_key}: {form_value} -> {form_value.upper()}")

else:
	print('<h1>405 Method Not Allowed</h1>')
	print('<p>The method that is being used to access the endpoint is not allowed</p>')

print('</body>')
print('</html>')
