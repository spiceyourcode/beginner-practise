import openai

# Set up your API key
openai.api_key = "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"

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
