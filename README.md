# Black-Scholes Options Pricing System

A production-grade C++ implementation of the Black-Scholes option pricing model with comprehensive testing, logging, configuration management, and performance profiling.

## 🚀 Features

### Core Functionality
- **Black-Scholes Option Pricing** - European calls and puts with full Greeks
- **Implied Volatility Calculation** - Newton-Raphson solver with convergence guarantees
- **Risk Analytics** - Comprehensive Greeks calculation and validation
- **Streamlit Web Interface** - Interactive options pricing with P&L heatmaps

### Production-Grade Infrastructure
- **Unit Testing Framework** - Comprehensive test coverage with performance benchmarks
- **Logging System** - Thread-safe logging with rotation and multiple output destinations
- **Configuration Management** - JSON-based config with environment variable overrides
- **Memory Profiling** - Leak detection and usage monitoring
- **Performance Profiling** - Execution timing and bottleneck identification
- **Thread Safety** - Full thread-safe implementation with concurrent testing

### Quality Assurance
- **Static Analysis** - Automated code quality checks
- **Memory Leak Detection** - Valgrind integration for leak detection
- **Thread Safety Analysis** - Helgrind integration for race condition detection
- **Code Coverage** - GCOV/LCOV integration for coverage reporting
- **Continuous Integration** - Automated testing and validation

## 📋 Requirements

### System Requirements
- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.10+** or **Make**
- **Python 3.7+** (for Streamlit interface)

### Development Tools (Optional)
- **Valgrind** - Memory leak detection
- **Cppcheck** - Static analysis
- **Clang-format** - Code formatting
- **LCOV** - Code coverage
- **Doxygen** - Documentation generation

## 🛠️ Installation

### Quick Start
```bash
# Clone repository
git clone <repository-url>
cd black-scholes-options

# Install dependencies (Ubuntu/Debian)
make install-deps

# Build release version
make release

# Run unit tests
make test

# Start Streamlit web interface
make streamlit
```

### Manual Installation
```bash
# Install system dependencies
sudo apt-get install build-essential g++ cmake valgrind cppcheck clang-format lcov doxygen

# Install Python dependencies
pip3 install streamlit plotly pandas numpy scipy pytest

# Build the project
make release
```

## 🏗️ Build System

### Available Build Targets

#### Core Builds
```bash
make release          # Optimized production build
make debug           # Debug build with sanitizers
make profile         # Profiling build with gprof
make test           # Build and run unit tests
```

#### Quality Assurance
```bash
make analyze        # Static code analysis
make format         # Code formatting
make memcheck       # Memory leak detection
make thread-check   # Thread safety analysis
make coverage       # Code coverage report
```

#### Performance & Benchmarking
```bash
make benchmark      # Performance benchmarks
make perf          # Performance profiling
```

#### Documentation & Packaging
```bash
make docs          # Generate documentation
make package       # Create distribution package
make docker        # Build Docker image
```

#### Maintenance
```bash
make clean         # Clean build artifacts
make distclean     # Deep clean
make help          # Show all available targets
```

## 🧪 Testing

### Unit Tests
```bash
# Run all unit tests
make test

# Run specific test suite
./bin/test_runner --suite BlackScholesOptionPricing

# Run tests with specific tag
./bin/test_runner --tag performance
```

### Test Coverage
```bash
# Generate coverage report
make coverage

# View coverage report
open coverage_html/index.html
```

### Memory Testing
```bash
# Check for memory leaks
make memcheck

# Profile memory usage
make debug
./bin/black_scholes --enable-memory-profiling
```

### Performance Testing
```bash
# Run benchmarks
make benchmark

# Profile performance
make perf
```

## ⚙️ Configuration

### Configuration File (`config.json`)
```json
{
  "monte_carlo": {
    "simulations": 100000,
    "steps": 252
  },
  "logging": {
    "level": "INFO",
    "file": "quantlib.log"
  },
  "threading": {
    "max_threads": 8
  }
}
```

### Environment Variables
```bash
export QUANTLIB_MONTE_CARLO_SIMULATIONS=500000
export QUANTLIB_LOGGING_LEVEL=DEBUG
export QUANTLIB_THREADING_MAX_THREADS=16
```

## 🖥️ Usage

### C++ Library
```cpp
#include "src/models/black_scholes.hpp"

using namespace BlackScholes;

// Create parameters
Parameters params(100.0, 100.0, 0.25, 0.05, 0.20, 0.0);

// Price call option
auto result = OptionPricer::price_call(params);
std::cout << "Call price: $" << result.price << std::endl;
std::cout << "Delta: " << result.greeks.delta << std::endl;

// Calculate implied volatility
double market_price = 5.0;
double implied_vol = OptionPricer::calculate_implied_volatility(
    market_price, params, true);
```

### Command Line Interface
```bash
# Interactive mode
./bin/black_scholes

# Batch mode
./bin/black_scholes --spot 100 --strike 105 --time 0.25 --rate 0.05 --vol 0.20
```

### Streamlit Web Interface
```bash
# Start web application
make streamlit

# Or directly
python3 -m streamlit run app.py
```

## 📊 Web Interface Features

### Interactive Options Pricing
- Real-time Black-Scholes pricing
- Greeks calculation and display
- Parameter sensitivity analysis

### Enhanced P&L Heatmap
- Multiple visualization types (P&L, Option Price, % P&L)
- 6 color schemes for different preferences
- Contour lines and breakeven analysis
- Moneyness indicators (ATM, ITM, OTM lines)
- Risk analytics dashboard

### Advanced Analytics
- Volatility surface generation
- Scenario analysis tools
- Portfolio management
- Risk metrics calculation

## 🔧 Architecture

### Project Structure
```
black-scholes-options/
├── src/
│   ├── models/          # Core pricing models
│   ├── utils/           # Utilities (logging, profiling)
│   ├── config/          # Configuration management
│   └── main.cpp         # Main application
├── tests/               # Unit tests
├── docs/               # Documentation
├── config.json         # Configuration file
├── Makefile           # Build system
└── app.py             # Streamlit web interface
```

### Key Components

#### Black-Scholes Model (`src/models/black_scholes.hpp`)
- Thread-safe option pricing
- Full Greeks calculation
- Implied volatility solver
- Parameter validation

#### Logging System (`src/utils/logger.hpp`)
- Thread-safe logging
- Multiple output destinations
- Log rotation and archiving
- Performance timing integration

#### Configuration Management (`src/config/config.hpp`)
- JSON configuration files
- Environment variable overrides
- Runtime configuration updates
- Thread-safe access

#### Memory Profiler (`src/utils/memory_profiler.hpp`)
- Allocation tracking
- Leak detection
- Usage statistics
- Custom allocators

#### Testing Framework (`tests/test_framework.hpp`)
- Lightweight unit testing
- Performance benchmarking
- Parameterized tests
- Thread safety testing

## 🚀 Performance

### Benchmarks (Release Build)
- **Option Pricing**: ~10,000 calculations/second
- **Greeks Calculation**: ~8,000 calculations/second
- **Implied Volatility**: ~1,000 calculations/second
- **Memory Usage**: <50MB typical, <100MB peak

### Optimization Features
- **Compiler Optimizations**: -O3, -march=native, -flto
- **Mathematical Optimizations**: Efficient normal distribution functions
- **Memory Optimizations**: Custom allocators, object pooling
- **Threading**: Parallel Monte Carlo simulations

## 🔒 Thread Safety

All core components are thread-safe:
- **Option Pricing**: Stateless calculations
- **Logging**: Thread-safe with mutex protection
- **Configuration**: Read-write locks for concurrent access
- **Memory Profiling**: Atomic operations and mutexes

### Thread Safety Testing
```bash
# Run thread safety analysis
make thread-check

# Run concurrent unit tests
./bin/test_runner --suite BlackScholesThreadSafety
```

## 📈 Production Deployment

### Docker Deployment
```bash
# Build Docker image
make docker

# Run container
docker run -p 8501:8501 black-scholes-options
```

### Performance Monitoring
```bash
# Enable performance logging
export QUANTLIB_PERFORMANCE_ENABLE_LOGGING=true

# Enable memory profiling
export QUANTLIB_MEMORY_ENABLE_PROFILING=true

# Run with monitoring
./bin/black_scholes --monitor
```

### Health Checks
```bash
# System health check
./bin/black_scholes --health-check

# Memory usage check
./bin/black_scholes --memory-check

# Performance benchmark
./bin/black_scholes --benchmark
```

## 🐛 Debugging

### Debug Build
```bash
# Build with debug symbols and sanitizers
make debug

# Run with AddressSanitizer
./bin/black_scholes
```

### Memory Debugging
```bash
# Check for memory leaks
make memcheck

# Profile memory usage
valgrind --tool=massif ./bin/black_scholes
```

### Performance Debugging
```bash
# Profile with gprof
make perf

# Profile with perf
perf record ./bin/black_scholes
perf report
```

## 📚 Documentation

### Generate Documentation
```bash
# Generate Doxygen documentation
make docs

# View documentation
open docs/html/index.html
```

### API Reference
- **BlackScholes::OptionPricer** - Main pricing interface
- **BlackScholes::Parameters** - Parameter validation and storage
- **BlackScholes::Greeks** - Risk sensitivities
- **Utils::Logger** - Logging interface
- **Config::ConfigManager** - Configuration management

## 🤝 Contributing

### Development Workflow
1. **Fork** the repository
2. **Create** feature branch (`git checkout -b feature/amazing-feature`)
3. **Write** tests for new functionality
4. **Ensure** all tests pass (`make test`)
5. **Run** static analysis (`make analyze`)
6. **Format** code (`make format`)
7. **Commit** changes (`git commit -m 'Add amazing feature'`)
8. **Push** to branch (`git push origin feature/amazing-feature`)
9. **Create** Pull Request

### Code Standards
- **C++17** standard compliance
- **Google C++ Style Guide** formatting
- **100% test coverage** for new features
- **Thread safety** for all public APIs
- **Comprehensive documentation** for public interfaces

### Testing Requirements
- Unit tests for all new functionality
- Performance benchmarks for critical paths
- Thread safety tests for concurrent code
- Memory leak tests for resource management

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Black-Scholes Model** - Fischer Black, Myron Scholes, Robert Merton
- **Numerical Methods** - Various academic and industry sources
- **Testing Framework** - Inspired by Google Test and Catch2
- **Logging System** - Inspired by spdlog and glog

## 📞 Support

For support, please:
1. Check the [documentation](docs/)
2. Search [existing issues](issues/)
3. Create a [new issue](issues/new) with detailed information
4. Include system information and reproduction steps

---

**Built with ❤️ for quantitative finance professionals**