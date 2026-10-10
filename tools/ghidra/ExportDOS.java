// Export DOS decompiler output for manual analysis; this is not compilable C++.
// @category MS-DOS-cpp
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

public class ExportDOS extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) throw new IllegalArgumentException("ExportDOS OUTPUT.c com|exe");
        if (args[1].equals("com")) {
            var entry = toAddr(0x100);
            if (!currentProgram.getMemory().contains(entry)) throw new IllegalStateException("COM origin 100H missing");
            disassemble(entry);
            if (getFunctionAt(entry) == null) createFunction(entry, "dos_com_entry");
            analyzeChanges(currentProgram);
        }
        DecompInterface decompiler = new DecompInterface();
        StringBuilder output = new StringBuilder();
        output.append("/* Ghidra analysis output, not a completed C++ port.\n")
            .append(" * Program: ").append(currentProgram.getName()).append("\n")
            .append(" * Language: ").append(currentProgram.getLanguageID()).append("\n")
            .append(" * Original Microsoft material: MIT License.\n */\n\n");
        int completed = 0, failed = 0;
        try {
            if (!decompiler.openProgram(currentProgram)) throw new IllegalStateException(decompiler.getLastMessage());
            FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
            while (functions.hasNext() && !monitor.isCancelled()) {
                Function function = functions.next();
                DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
                output.append("/* Entry ").append(function.getEntryPoint()).append(" */\n");
                if (result.decompileCompleted() && result.getDecompiledFunction() != null) {
                    output.append(result.getDecompiledFunction().getC()).append("\n");
                    completed++;
                } else {
                    output.append("/* DECOMPILATION FAILED: ").append(function.getName()).append(" */\n");
                    failed++;
                    printerr(function.getName() + ": " + result.getErrorMessage());
                }
            }
        } finally { decompiler.dispose(); }
        Files.writeString(Path.of(args[0]), output.toString(), StandardCharsets.UTF_8);
        println("Exported " + completed + " functions; " + failed + " failures.");
        if (completed == 0 || failed != 0 || monitor.isCancelled())
            throw new IllegalStateException("Incomplete decompilation; inspect output and log");
    }
}
