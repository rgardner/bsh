FROM gcc:9.2

# Update and install dependencies, and remove the package manager cache. Do
# this in a single step for better caching.
RUN apt-get --yes update && apt-get install --yes --no-install-recommends \
        cmake \
        libbsd-dev \
        libreadline-dev \
        ninja-build \
        pkg-config \
        && rm -rf /var/lib/apt/lists/*
