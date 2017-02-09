import nltk
import tokenize

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        (posList, negList) = ([], [])
        (posFile, negFile) = (open(positives), open(negatives))
        for feeling in [(posList, posFile), (negList, negFile)]:
            for line in feeling[1]:
                if(not line.startswith(";") and len(line.strip()) > 0):
                    feeling[0].append(line.strip())

        posFile.close(), negFile.close()
        (self.positives, self.negatives) = (posList, negList)
    

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        tokenizer = nltk.tokenize.TweetTokenizer()

        score = 0
        tokens = tokenizer.tokenize(text)
        for token in tokens:
            if token in self.positives:
                score += 1
            elif token in self.negatives:
                score += -1
                    
        return score
