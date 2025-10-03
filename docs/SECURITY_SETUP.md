# GitHub Advanced Security Setup Guide

This document explains the security features enabled for the poker-hand-evaluation-c project and how to monitor them.

## Table of Contents

- [Overview](#overview)
- [Enabled Security Features](#enabled-security-features)
- [What Each Feature Does](#what-each-feature-does)
- [Where to Check Security Status](#where-to-check-security-status)
- [Understanding CodeQL Alerts](#understanding-codeql-alerts)
- [Workflow Permissions Fix](#workflow-permissions-fix)
- [Responding to Security Alerts](#responding-to-security-alerts)

## Overview

GitHub Advanced Security (GHAS) is **free for public repositories**. This project uses several GHAS features to detect security vulnerabilities, secret leaks, and maintain code quality.

## Enabled Security Features

| Feature | Status | Purpose |
|---------|--------|---------|
| **Secret Scanning** | ✅ Enabled | Detects API keys, tokens, credentials in commits |
| **Secret Push Protection** | ✅ Enabled | Blocks commits containing secrets |
| **CodeQL Analysis** | ✅ Enabled | Scans code for security vulnerabilities |
| **Dependabot Alerts** | ⚠️ Recommended | Alerts for vulnerable dependencies |
| **Private Vulnerability Reporting** | ⚠️ Recommended | Allows researchers to report vulnerabilities privately |

## What Each Feature Does

### 1. Secret Scanning

**What it does:**
- Scans all commits for known secret patterns (API keys, tokens, passwords, private keys)
- Automatically detects secrets from 200+ service providers (AWS, GitHub, Google, etc.)
- Alerts you when secrets are found

**Where to check:**
```
Repository → Security → Secret scanning
URL: https://github.com/arthurvanacker/poker-hand-evaluation-c/security/secret-scanning
```

**Example detections:**
- AWS Access Keys
- GitHub Personal Access Tokens
- Private SSH keys
- Database connection strings
- OAuth tokens

### 2. Secret Push Protection

**What it does:**
- **Blocks commits** containing secrets BEFORE they reach GitHub
- Prevents secrets from entering git history
- Works locally when you `git push`

**What you'll see if blocked:**
```bash
$ git push
remote: error: GH013: Secret detected in commit
remote:
remote: - GitHub Personal Access Token found in config.yml:15
remote:
To github.com:arthurvanacker/poker-hand-evaluation-c.git
 ! [remote rejected] main -> main (push declined due to secret detection)
```

**How to fix:**
1. Remove the secret from your code
2. Use environment variables or secret management instead
3. Commit the cleaned code
4. Push again

### 3. CodeQL Analysis

**What it does:**
- Static code analysis for **security vulnerabilities**
- Scans both **C source code** and **GitHub Actions workflows**
- Runs automatically on:
  - Every push to `main`
  - Every pull request
  - Weekly schedule (Mondays at 00:00 UTC)

**What it detects in C code:**
- Buffer overflows (`strcpy` without bounds checking)
- Null pointer dereferences
- Use-after-free vulnerabilities
- Memory leaks
- Integer overflows
- Format string vulnerabilities
- Uninitialized variables
- Unsafe type casts

**What it detects in workflows:**
- Missing permissions blocks (CWE-275)
- Excessive privileges
- Insecure workflow configurations

**Where to check:**
```
Repository → Security → Code scanning
URL: https://github.com/arthurvanacker/poker-hand-evaluation-c/security/code-scanning
```

**Workflow location:**
- CodeQL runs as part of GitHub's built-in scanning (no workflow file needed for basic scanning)
- Custom analysis can be added via `.github/workflows/codeql.yml`

### 4. Dependabot (Recommended to Enable)

**What it does:**
- Monitors dependencies for known vulnerabilities
- Creates automated pull requests to update vulnerable dependencies
- For this C project: mainly tracks GitHub Actions dependencies

**How to enable:**
1. Go to: Settings → Code security and analysis
2. Enable "Dependabot alerts"
3. Enable "Dependabot security updates"

**Where to check:**
```
Repository → Security → Dependabot alerts
URL: https://github.com/arthurvanacker/poker-hand-evaluation-c/security/dependabot
```

### 5. Private Vulnerability Reporting (Recommended to Enable)

**What it does:**
- Provides a private channel for security researchers to report vulnerabilities
- Prevents public disclosure before you can fix the issue
- Creates a structured reporting process

**How to enable:**
1. Go to: Settings → Code security and analysis
2. Enable "Private vulnerability reporting"

**Where reports appear:**
```
Repository → Security → Advisories
URL: https://github.com/arthurvanacker/poker-hand-evaluation-c/security/advisories
```

## Where to Check Security Status

### Main Security Dashboard

```
Repository → Security tab
URL: https://github.com/arthurvanacker/poker-hand-evaluation-c/security
```

This shows:
- Overview of all security features
- Number of open alerts
- Recent security activity

### Code Scanning Alerts

```
Repository → Security → Code scanning
URL: https://github.com/arthurvanacker/poker-hand-evaluation-c/security/code-scanning
```

**What you'll see:**
- List of all security vulnerabilities found by CodeQL
- Severity: Critical / High / Medium / Low
- Status: Open / Fixed / Dismissed
- File location and line number

**Example alert:**
```
High severity: Potential buffer overflow in parse_card
├─ File: src/parser.c:42
├─ Issue: Using strcpy without bounds checking
└─ Recommendation: Use strncpy or check length first
```

### Secret Scanning Alerts

```
Repository → Security → Secret scanning
URL: https://github.com/arthurvanacker/poker-hand-evaluation-c/security/secret-scanning
```

**What you'll see:**
- Detected secrets with commit SHA
- Secret type (GitHub token, AWS key, etc.)
- Status: Open / Resolved / Revoked

### GitHub Actions Workflows

```
Repository → Actions
URL: https://github.com/arthurvanacker/poker-hand-evaluation-c/actions
```

**Security-related workflows:**
- **CI** (`.github/workflows/ci.yml`) - Includes cppcheck static analysis, Valgrind memory checks
- **CodeQL** - Runs automatically (dynamic workflow managed by GitHub)

**Check workflow status:**
```bash
gh run list --workflow=CodeQL --limit 5
gh run list --workflow=CI --limit 5
```

## Understanding CodeQL Alerts

### Alert Lifecycle

1. **CodeQL scans code** → Finds potential vulnerability
2. **Alert created** → Appears in Security tab (status: Open)
3. **Developer fixes** → Creates PR with fix
4. **PR merged** → Fix lands in main branch
5. **CodeQL re-scans** → Verifies fix (happens automatically on next push)
6. **Alert auto-closes** → Status changes to "Fixed"

### Alert Details

Each alert shows:

**Severity Levels:**
- **Critical**: Exploitable vulnerabilities, immediate action required
- **High**: Serious issues, fix soon
- **Medium**: Moderate issues, fix in upcoming sprint
- **Low**: Minor issues, fix when convenient

**Information Provided:**
- File path and line number
- Code snippet showing the issue
- Explanation of the vulnerability
- Recommended fix
- CWE reference (Common Weakness Enumeration)
- Data flow visualization (how data flows to create the vulnerability)

### Viewing Alerts in CLI

```bash
# List all open code scanning alerts
gh api repos/arthurvanacker/poker-hand-evaluation-c/code-scanning/alerts \
  --jq '.[] | select(.state=="open") | {number, severity: .rule.security_severity_level, rule: .rule.description}'

# View specific alert details
gh api repos/arthurvanacker/poker-hand-evaluation-c/code-scanning/alerts/1

# List secret scanning alerts
gh api repos/arthurvanacker/poker-hand-evaluation-c/secret-scanning/alerts
```

## Workflow Permissions Fix

### What Was Fixed (PR #114)

**Problem:** GitHub Actions workflows had no explicit `permissions:` blocks, violating the principle of least privilege.

**Risk:** Jobs inherited repository-wide permissions (potentially read-write access), giving them more access than needed.

**Fix:** Added explicit `permissions:` to all 8 jobs in `.github/workflows/ci.yml`:

```yaml
jobs:
  build-gcc:
    name: Build with GCC
    runs-on: ubuntu-latest
    permissions:
      contents: read  # Only read repository code
    steps:
      # ... build steps
```

**Permissions assigned:**

| Job | Permissions | Reason |
|-----|-------------|--------|
| build-gcc | `contents: read` | Only needs to read code |
| lint | `contents: read` | Only needs to read code |
| build-clang | `contents: read` | Only needs to read code |
| test | `contents: read` | Only needs to read code |
| valgrind | `contents: read` | Only needs to read code |
| fuzzing | `contents: read` | Only needs to read code |
| coverage | `contents: read`<br>`actions: write` | Reads code + uploads artifacts |
| build-status | `contents: read`<br>`actions: read` | Reads code + checks job statuses |

### Why This Matters

**Security Principle: Least Privilege**
- Jobs should only have the minimum permissions needed
- Prevents accidental or malicious code from accessing sensitive resources
- Reduces attack surface if workflow is compromised

**What could happen without explicit permissions:**
- A compromised dependency could push malicious commits
- Workflow could modify repository settings
- Secrets could be exfiltrated with write access

**With explicit permissions:**
- Jobs can only read code (no write access)
- Attack surface minimized
- Complies with security best practices

### Verification

**Check that permissions are applied:**

```bash
# View workflow file
cat .github/workflows/ci.yml | grep -A 2 "permissions:"

# Verify CodeQL alerts are closed
gh api repos/arthurvanacker/poker-hand-evaluation-c/code-scanning/alerts \
  --jq '.[] | select(.rule.id=="actions/missing-workflow-permissions") | {number, state}'
```

**Expected result:** All 8 alerts (#1-#8) should show `state: "fixed"` after CodeQL re-scans.

## Responding to Security Alerts

### When CodeQL Finds a Vulnerability

1. **Review the alert** in Security → Code scanning
2. **Understand the issue** (read explanation, check code location)
3. **Create an issue** (optional, for tracking)
4. **Fix the vulnerability** in a new branch
5. **Create PR** with the fix
6. **Merge after review**
7. **Alert auto-closes** when CodeQL re-scans

### When a Secret is Detected

1. **DON'T PANIC** - Secret scanning prevents the push
2. **Remove the secret** from your code
3. **Revoke/rotate the secret** (GitHub token, API key, etc.)
4. **Use environment variables** or a secrets manager instead
5. **Commit and push** the cleaned code

**If secret already pushed:**
```bash
# Use git filter-repo to remove from history
git filter-repo --path config.yml --invert-paths
git push --force
```

**Then immediately:**
1. Revoke the compromised secret (GitHub, AWS, etc.)
2. Rotate all related credentials
3. Monitor for unauthorized access

### When Dependabot Finds a Vulnerability

1. **Review the alert** in Security → Dependabot
2. **Check the PR** (Dependabot auto-creates update PR)
3. **Review changes** in the PR
4. **Test locally** (if needed)
5. **Merge the PR** to apply the fix

## Monitoring Best Practices

### Daily

- Check Security tab for new alerts
- Review failed workflow runs

### Weekly

- Review all open security alerts
- Triage and prioritize fixes
- Check CodeQL scheduled scan results (runs Mondays)

### Per Release

- Ensure all Critical/High severity alerts are resolved
- Review and accept/dismiss Low severity alerts
- Document any accepted risks

### CLI Monitoring Commands

```bash
# Check for open security alerts
gh api repos/arthurvanacker/poker-hand-evaluation-c/code-scanning/alerts \
  --jq 'map(select(.state=="open")) | length'

# List critical/high severity alerts
gh api repos/arthurvanacker/poker-hand-evaluation-c/code-scanning/alerts \
  --jq '.[] | select(.state=="open" and (.rule.security_severity_level=="critical" or .rule.security_severity_level=="high"))'

# Check recent workflow runs
gh run list --limit 5

# View security overview
gh api repos/arthurvanacker/poker-hand-evaluation-c | jq .security_and_analysis
```

## Additional Resources

- [GitHub Code Scanning Docs](https://docs.github.com/en/code-security/code-scanning)
- [CodeQL for C/C++](https://codeql.github.com/docs/codeql-language-guides/codeql-for-cpp/)
- [Secret Scanning](https://docs.github.com/en/code-security/secret-scanning)
- [Workflow Permissions](https://docs.github.com/en/actions/using-jobs/assigning-permissions-to-jobs)
- [CWE Database](https://cwe.mitre.org/) - Vulnerability classifications

---

**Last Updated:** October 4, 2025
**Maintained By:** arthurvanacker
