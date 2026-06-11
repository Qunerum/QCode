```c
#define STATUS "STABLE"
#define LICENSE "GNU GPL v3"

typedef struct {
    char* name;
    char* version;
    char* paradigm;
    char* core_language;
    char* about;
    char* features[4];
    char* quick_start[2];
    char* code_example[4];
} Project;

int main() {
    Project qcode = {
        .name = "QCode",
        .version = "0.2.2",
        .paradigm = "Hybrid (Assembly & Python)",
        .core_language = "Pure C",
        .about = "A minimalist, fast, string-based interpreter with a custom memory manager.",
        
        .features = {
            "Custom Heap Memory Manager (kmalloc / kfree)",
            "Dynamic lists and safe string buffer processing",
            "Low-level control with high-level ease of use",
            "Built-in library system via 'include' command"
        },
        .quick_start = {
            "Build command: make",
            "Run command:   ./qcode"
        },
        .code_example = {
            "int a 10",
            "int b 5",
            "add a v(b)",
            "println v(a)" // Output: 15
        }
    };
    return 0;
}
```
