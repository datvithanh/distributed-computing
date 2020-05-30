set -e

wget https://snap.stanford.edu/data/twitter.tar.gz
tar xvzf twitter.tar.gz
mv twitter egos
rm twitter.tar.gz

wget https://snap.stanford.edu/data/twitter_combined.txt.gz
gunzip twitter_combined.txt.gz

wget https://snap.stanford.edu/data/bigdata/communities/com-dblp.ungraph.txt.gz
gunzip com-dblp.ungraph.txt.gz

wget https://snap.stanford.edu/data/bigdata/communities/com-youtube.ungraph.txt.gz
gunzip com-youtube.ungraph.txt.gz