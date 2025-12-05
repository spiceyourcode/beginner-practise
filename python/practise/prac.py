import openai

# Set up your API key
openai.api_key = "your-api-key"

# Call the OpenAI ChatCompletion API (new syntax)
response = openai.ChatCNompletion.create(
    model="gpt-3.5-turbo",
    messages=[
        {"role": "system", "content": "You are a helpful assistant."},
        {"role": "user", "content": "Tell me a joke."}
    ]
)

# Print the generated response
print(response.choices[0].message['content'])
