import React from 'react';

export default function Footer() {
    return (
        <footer className="w-full h-16 border-t border-bio-border bg-bio-bg flex items-center justify-center text-slate-500 text-sm mt-auto z-10 relative">
            <p>
                <span className="font-semibold text-slate-400">GenomeSift</span> — Built with C++17 and React
            </p>
        </footer>
    );
}
