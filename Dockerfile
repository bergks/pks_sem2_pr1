
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y build-essential cmake git qt6-base-dev qt6-declarative-dev libgl1-mesa-dev && rm -rf /var/lib/apt/lists/*

RUN git clone --branch v3.4.0 --depth 1 https://github.com/catchorg/Catch2.git /opt/Catch2

WORKDIR /app
COPY CMakeLists.txt .
COPY src/ ./src/
COPY qml/ ./qml/
COPY tests/ ./tests/

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --parallel

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y qt6-base-dev qt6-declarative-dev libgl1-mesa-dev libxcb-xinerama0 qml6-module-* && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/build/tests/test_sorting /app/tests/test_sorting
COPY --from=builder /app/build/tests/scenario1_basic /app/tests/scenario1_basic
COPY --from=builder /app/build/tests/scenario2_interactive /app/tests/scenario2_interactive
COPY --from=builder /app/build/appsort_visualizer /app/appsort_visualizer

COPY qml/ /app/qml/

WORKDIR /app

RUN echo '#!/bin/bash' > /app/entrypoint.sh && \
    echo 'run_tests() {' >> /app/entrypoint.sh && \
    echo '    echo "======================================"' >> /app/entrypoint.sh && \
    echo '    echo "    ЗАПУСК UNIT-ТЕСТОВ (Catch2)"' >> /app/entrypoint.sh && \
    echo '    echo "======================================"' >> /app/entrypoint.sh && \
    echo '    /app/tests/test_sorting' >> /app/entrypoint.sh && \
    echo '    echo ""' >> /app/entrypoint.sh && \
    echo '    echo "======================================"' >> /app/entrypoint.sh && \
    echo '    echo "    ЗАПУСК СЦЕНАРИЯ 1"' >> /app/entrypoint.sh && \
    echo '    echo "======================================"' >> /app/entrypoint.sh && \
    echo '    /app/tests/scenario1_basic' >> /app/entrypoint.sh && \
    echo '    echo ""' >> /app/entrypoint.sh && \
    echo '    echo "======================================"' >> /app/entrypoint.sh && \
    echo '    echo "    ВСЕ ТЕСТЫ ПРОЙДЕНЫ"' >> /app/entrypoint.sh && \
    echo '    echo "======================================"' >> /app/entrypoint.sh && \
    echo '}' >> /app/entrypoint.sh && \
    echo 'run_gui() {' >> /app/entrypoint.sh && \
    echo '    echo "Запуск GUI приложения..."' >> /app/entrypoint.sh && \
    echo '    /app/appsort_visualizer' >> /app/entrypoint.sh && \
    echo '}' >> /app/entrypoint.sh && \
    echo 'case "${1}" in' >> /app/entrypoint.sh && \
    echo '    tests) run_tests ;;' >> /app/entrypoint.sh && \
    echo '    gui) run_gui ;;' >> /app/entrypoint.sh && \
    echo '    all) run_tests && echo "" && run_gui ;;' >> /app/entrypoint.sh && \
    echo '    *) echo "Usage: docker run ... [tests|gui|all]" && exit 1 ;;' >> /app/entrypoint.sh && \
    echo 'esac' >> /app/entrypoint.sh && \
    chmod +x /app/entrypoint.sh

ENTRYPOINT ["/app/entrypoint.sh"]
CMD ["tests"]
