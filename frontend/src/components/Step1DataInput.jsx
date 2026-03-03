import React, { useState } from 'react';

export default function Step1DataInput({
    samples,
    referenceFile,
    setReferenceFile,
    patientFile,
    setPatientFile,
    onContinue
}) {
    const [refUploading, setRefUploading] = useState(false);
    const [patUploading, setPatUploading] = useState(false);

    const handleFileUpload = async (event, type) => {
        const file = event.target.files[0];
        if (!file) return;

        const setLoader = type === 'reference' ? setRefUploading : setPatUploading;
        const setFile = type === 'reference' ? setReferenceFile : setPatientFile;

        setLoader(true);

        const formData = new FormData();
        formData.append('file', file);
        formData.append('type', type);

        try {
            const res = await fetch('/api/upload', {
                method: 'POST',
                body: formData
            });
            const data = await res.json();

            if (data.status !== 'error') {
                setFile({
                    name: data.name,
                    description: data.description,
                    path: data.path,
                    // Since it's custom upload we might not have a preview initially, but we can simulate it for demo
                    sequence_preview: "..."
                });
            } else {
                alert(data.message);
            }
        } catch (e) {
            console.error(e);
            alert("Failed to upload file");
        } finally {
            setLoader(false);
        }
    };

    const ColorizedSequence = () => (
        <div className="font-mono text-sm bg-bio-bg p-3 rounded border border-bio-border flex flex-wrap gap-[1px]">
            {'ATGGTGCATCTGACTCCTGAGGAGAAGTCTGCCGTTACTGCCCTGTGGGG'.split('').map((char, i) => {
                let colorClass = 'text-slate-400';
                if (char === 'A') colorClass = 'text-bio-green font-bold';
                if (char === 'T') colorClass = 'text-bio-red font-bold';
                if (char === 'G') colorClass = 'text-bio-amber font-bold';
                if (char === 'C') colorClass = 'text-bio-cyan font-bold';
                return <span key={i} className={colorClass}>{char}</span>;
            })}
            <span className="text-slate-500 ml-2">...</span>
        </div>
    );

    return (
        <div className="bg-bio-card/80 border border-bio-border rounded-xl p-8 shadow-xl">
            <div className="flex items-center gap-4 mb-8">
                <div className="flex items-center justify-center w-10 h-10 rounded-full bg-bio-cyan/20 border border-bio-cyan text-bio-cyan font-bold text-xl glow-cyan">
                    1
                </div>
                <h2 className="text-3xl font-bold">Load Sequences</h2>
            </div>

            <div className="grid grid-cols-1 lg:grid-cols-2 gap-8">

                {/* Reference Column */}
                <div className="bg-bio-bg/50 border border-bio-border rounded-lg p-6 flex flex-col h-full hover:border-bio-cyan/30 transition-colors">
                    <h3 className="text-xl font-bold text-bio-cyan mb-4 flex items-center gap-2">
                        Reference Sequence
                        <span className="text-xs bg-bio-cyan/10 text-bio-cyan px-2 py-1 rounded-md uppercase tracking-wider">Healthy</span>
                    </h3>

                    <select
                        className="w-full bg-bio-card border border-bio-border text-slate-200 rounded-md p-3 mb-4 focus:outline-none focus:border-bio-cyan"
                        value={referenceFile ? referenceFile.name : ''}
                        onChange={(e) => {
                            const selected = samples.references.find(s => s.name === e.target.value);
                            if (selected) setReferenceFile(selected);
                        }}
                    >
                        <option value="" disabled>-- Select a Reference Sample --</option>
                        {samples.references.map(s => (
                            <option key={s.name} value={s.name}>{s.name} - {s.description}</option>
                        ))}
                    </select>

                    <div className="relative my-2">
                        <div className="absolute inset-0 flex items-center">
                            <div className="w-full border-t border-bio-border"></div>
                        </div>
                        <div className="relative flex justify-center text-sm">
                            <span className="px-2 bg-bio-bg/50 text-slate-400">OR</span>
                        </div>
                    </div>

                    <div className="border-2 border-dashed border-bio-border hover:border-bio-cyan/50 rounded-md p-4 flex justify-center items-center cursor-pointer transition-colors mt-2 mb-6 group relative">
                        <input type="file" accept=".fasta,.fa" className="absolute inset-0 w-full h-full opacity-0 cursor-pointer" onChange={(e) => handleFileUpload(e, 'reference')} />
                        <span className="text-slate-400 group-hover:text-bio-cyan font-medium">
                            {refUploading ? 'Uploading...' : 'Click or drag FASTA file here'}
                        </span>
                    </div>

                    {referenceFile && (
                        <div className="mt-auto bg-black/30 rounded-lg p-4 border border-bio-cyan/20">
                            <div className="flex justify-between items-start mb-2">
                                <div>
                                    <h4 className="font-bold text-white">{referenceFile.name}</h4>
                                    <p className="text-xs text-slate-400">{referenceFile.description || 'Custom Upload'}</p>
                                </div>
                                <span className="text-xs bg-bio-cyan/20 text-bio-cyan px-2 py-1 rounded">DNA</span>
                            </div>
                            <div className="mt-3">
                                <ColorizedSequence />
                            </div>
                        </div>
                    )}
                </div>

                {/* Patient Column */}
                <div className="bg-bio-bg/50 border border-bio-border rounded-lg p-6 flex flex-col h-full hover:border-[#a78bfa]/30 transition-colors">
                    <h3 className="text-xl font-bold text-[#a78bfa] mb-4 flex items-center gap-2">
                        Patient Sequence
                        <span className="text-xs bg-[#a78bfa]/10 text-[#a78bfa] px-2 py-1 rounded-md uppercase tracking-wider">Sample</span>
                    </h3>

                    <select
                        className="w-full bg-bio-card border border-bio-border text-slate-200 rounded-md p-3 mb-4 focus:outline-none focus:border-[#a78bfa]"
                        value={patientFile ? patientFile.name : ''}
                        onChange={(e) => {
                            const selected = samples.patients.find(s => s.name === e.target.value);
                            if (selected) setPatientFile(selected);
                        }}
                    >
                        <option value="" disabled>-- Select a Patient Sample --</option>
                        {samples.patients.map(s => (
                            <option key={s.name} value={s.name}>{s.name} - {s.description}</option>
                        ))}
                    </select>

                    <div className="relative my-2">
                        <div className="absolute inset-0 flex items-center">
                            <div className="w-full border-t border-bio-border"></div>
                        </div>
                        <div className="relative flex justify-center text-sm">
                            <span className="px-2 bg-bio-bg/50 text-slate-400">OR</span>
                        </div>
                    </div>

                    <div className="border-2 border-dashed border-bio-border hover:border-[#a78bfa]/50 rounded-md p-4 flex justify-center items-center cursor-pointer transition-colors mt-2 mb-6 group relative">
                        <input type="file" accept=".fasta,.fa" className="absolute inset-0 w-full h-full opacity-0 cursor-pointer" onChange={(e) => handleFileUpload(e, 'patient')} />
                        <span className="text-slate-400 group-hover:text-[#a78bfa] font-medium">
                            {patUploading ? 'Uploading...' : 'Click or drag FASTA file here'}
                        </span>
                    </div>

                    {patientFile && (
                        <div className="mt-auto bg-black/30 rounded-lg p-4 border border-[#a78bfa]/20">
                            <div className="flex justify-between items-start mb-2">
                                <div>
                                    <h4 className="font-bold text-white">{patientFile.name}</h4>
                                    <p className="text-xs text-slate-400">{patientFile.description || 'Custom Upload'}</p>
                                </div>
                                <span className="text-xs bg-[#a78bfa]/20 text-[#a78bfa] px-2 py-1 rounded">DNA</span>
                            </div>
                            <div className="mt-3">
                                <ColorizedSequence />
                            </div>
                        </div>
                    )}
                </div>

            </div>

            <div className="mt-10 flex justify-end">
                <button
                    onClick={onContinue}
                    disabled={!referenceFile || !patientFile}
                    className={`px-8 py-3 rounded-lg font-bold text-lg transition-all duration-300
            ${(!referenceFile || !patientFile)
                            ? 'bg-slate-700 text-slate-500 cursor-not-allowed'
                            : 'bg-bio-cyan text-bio-bg hover:bg-white hover:shadow-[0_0_15px_rgba(0,212,255,0.5)] transform hover:-translate-y-1'
                        }`}
                >
                    Continue Configuration →
                </button>
            </div>
        </div>
    );
}
