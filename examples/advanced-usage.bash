../chvojvoj \
    --url "http://www.cburch.com/logisim/index.html" \
    --recursive true \
    --max-recursion-depth 20 \
    --out-dir "../src/youll-never-find-me/out" \
    --download-images true \
    --thread-cnt 100 \
    --leaf-urls-to-local-file false \
    --accept-regex ".*\/logisim.*" \
    --reject-regex ".*\/docs\/(1|(2\.[^7])).*" # Ignore old docs
