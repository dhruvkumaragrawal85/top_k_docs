# Function to split a text document into smaller documents
def split_text_into_segments(input_file, output_prefix, segment_length):
    with open(input_file, "r") as file:
        text = file.read()
        words = text.split()

    num_segments = 5
    for i in range(num_segments):
        start = i * segment_length
        end = (i + 1) * segment_length
        segment_text = " ".join(words[start:end])

        output_file = f"{output_prefix}_{i + 1}.txt"
        with open(output_file, "w") as out_file:
            out_file.write(segment_text)


# Input file name and desired segment length (200 words)
input_file = "Algo2/text_corpus.txt"
segment_length = 300

# Output file prefix (e.g., 'doc' will generate 'doc_1.txt', 'doc_2.txt', ...)
output_prefix = "doc"

split_text_into_segments(input_file, output_prefix, segment_length)
