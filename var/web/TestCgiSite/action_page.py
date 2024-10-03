import os

# print("Content-type: text/html\r\n\r")
# Print HTML content
print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title>Environment Variables</title>")
print("</head>")
print("<body>")
print("<h1>Environment Variables:</h1>")
print("<ul>")
for key, value in os.environ.items():
    print(f"<li><strong>{key}:</strong> {value}</li>")
print("</ul>")
print("</body>")
print("</html>")