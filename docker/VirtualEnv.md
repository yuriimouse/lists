# Virtual environment
#### Docker compose and run

    cd docker
    docker compose up -d
    docker compose exec lists-lib /bin/bash
    cd /lists

#### Make bin|lib

    export LD_LIBRARY_PATH=/usr/local/custom/lib/`
- `make` basic make
- `make re` for remake
- `make test` for build tests
- `make install` for install
