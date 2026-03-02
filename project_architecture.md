# GenomeSift: Project Architecture & Development Log

## 1. System Architecture
GenomeSift is designed as a high-performance, three-tier bioinformatics pipeline built to calculate DNA sequence alignments, detect mutations, and synthesize protein structures.

### Tier 1: C++ Core Engine (High-Performance Backend)
The core engine is responsible for the computationally heavy task of genetic sequence alignment. 
- **Algorithms**: Implements classical Dynamic Programming algorithms: Needleman-Wunsch (Global Alignment) and Smith-Waterman (Local Alignment).
- **Sequence Processing**: Translates raw FASTA input into structured `DNASequence` objects. It further calculates the computational Central Dogma: transcribing DNA into `RNASequence` objects and translating that into `ProteinSequence` chains using standard codon tables.
- **Mutation Detection**: Compares the aligned Reference and Patient sequences base-by-base, identifying Single Nucleotide Polymorphisms (SNPs) as well as Insertion/Deletion (InDel) mutations.
- **Disease Mapping**: Queries identified mutations against a local CSV database (`disease_db.csv`) to map genetic variations to known pathogenic conditions.

### Tier 2: Python Flask API (Middleware Wrapper)
A lightweight REST API built with Flask.
- **Subprocess Orchestration**: Acts as a bridge between the web and the C++ binary. When a request hits `/api/align`, Flask executes `./genomesift.exe --json ...` using Python's `subprocess` module.
- **File Management**: Serves static FASTA files for the frontend's pre-populated dropdowns and handles incoming file uploads from the user.

### Tier 3: React Single Page Application (Frontend UI)
A modern, responsive, component-driven user interface.
- **Visualization**: React natively renders the complex outputs of the C++ engine. This includes the base-by-base visual alignment track, severity-colored mutation cards, and a massive dynamic programming heatmap table.
- **Styling**: Uses Tailwind CSS with a custom configuration to achieve a sleek, dark-mode bioinformatics laboratory aesthetic.

---

## 2. Development Error Log & Resolutions

During the construction of GenomeSift, several key technical hurdles were encountered and resolved:

### 1. Pointer Subtraction Typo in Base Initialization
- **Error**: `invalid conversion from 'const char*' to 'char'`
- **Cause**: In `DNASequence.cpp`, character validation was accidentally written as `if (base == "A" || base == "C"...)` using double quotes (string literals) instead of single quotes (char literals).
- **Resolution**: Updated to `if (base == 'A' || base == 'C'...)`.

### 2. C++17 Structured Binding Compilation Issues
- **Error**: `warning: structured bindings only available with '-std=c++17' or '-std=gnu++17'`
- **Cause**: Using `for (auto [row, col] : path)` which is a C++17 feature, but the compiler command was omitting the standard flag.
- **Resolution**: Updated the Makefile to explicitly use `-std=c++17`.

### 3. Signed/Unsigned Integer Comparison
- **Error**: `warning: comparison of integer expressions of different signedness: 'int' and 'size_t'`
- **Cause**: Loops in `MutationDetector.cpp` and `ReportGenerator.cpp` were comparing standard `int i` to `std::string::length()` (which returns `size_t`).
- **Resolution**: Casted the `int` to `size_t` where appropriate.

### 4. React Tailwind v4 to v3 Downgrade
- **Error**: `It looks like you're trying to use 'tailwindcss' directly as a PostCSS plugin`
- **Cause**: The Create React App (CRA) pipeline natively struggled to build the brand new Tailwind v4 release, causing PostCSS pipeline failures during `npm run build`.
- **Resolution**: Uninstalled the v4 plugin and explicitly installed `tailwindcss@^3.4.3` and downgraded the configuration syntax to stabilize the build.

### 5. C++ 0-Index vs Disease DB 1-Index Offset
- **Error**: Synthesized FASTA mutations were reading as "Unknown Clinical Significance" instead of "Hereditary Breast Cancer".
- **Cause**: A mismatch between the way humans count DNA positions (1-based index) and how C++ arrays find them (0-based index). The UI reported the mutation at position 187 because the Python script generated the mutation 2 slots too late in the array.
- **Resolution**: Adjusted the Python FASTA generator script to align perfectly with the 0-based index expected by the C++ engine logic.

---

## 3. How the "Other / Discover" Target Gene Variant Works

### What happens if you upload a random FASTA sequence from the internet?

GenomeSift is **not** hardcoded to only work on the 7 genes we provided. It is a generalized, universal alignment engine.

If you download a random gene from the NCBI database (for example, the *Insulin* gene) and upload it via drag-and-drop:

1. **Alignment & Processing**: The C++ engine will execute the Needleman-Wunsch algorithm exactly as normal. It will perfectly align the two sequences, calculate the optimal DP score, and transcribe the DNA into RNA and Protein.
2. **Mutation Detection**: It will successfully highlight every single mutation, SNP, insertion, and deletion between the two sequences.
3. **The "Other / Discover" Role**: Because your random internet sequence does not exist in our specific `disease_db.csv` mapping file, the engine will not know what *disease name* to assign the mutation to. 
4. **Graceful Fallback**: Under the "Other / Discover" mode, the engine gracefully falls back to tagging the severity as **LOW RISK** and the disease as **"Unknown Clinical Significance"**.

This simulates exactly how real-world exploratory bioinformatics works: researchers use pipelines like GenomeSift to align novel DNA, find the mutations, and then flag them as "Unknown Significance" until a wet-lab biologist can study what that specific physical mutation actually causes!
